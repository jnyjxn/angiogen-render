import os
import sys
import time

import pyrender
import importlib
import numpy as np
from tqdm import tqdm
from PIL import Image
import multiprocessing
from pathlib import Path
from skimage.transform import resize as sk_resize

from utils.MatrixCalculator import MatrixCalculator

class _Renderer(object):
	def __init__(self, SID=1040, beam_energy=150, pixel_size=(0.25, 0.25), image_size=(1526, 1496)):
		self.configuration = {
			"angle": [0, 0],        #(Positioner Primary Angle, Positioner Secondary Angle)
			"position": [0, 0, 0]   #(TableX, TableY, TableZ)
		}

		self.SID = SID
		self.beam_energy = beam_energy
		self.pixel_size	= pixel_size
		self.image_size = image_size

	@property
	def pose(self):
		return MatrixCalculator.DICOMtoPose(
			self.configuration["angle"][0],
			self.configuration["angle"][1],
			self.SID,
			self.configuration["position"]
		)

	@property
	def cameraMatrix(self):
		return MatrixCalculator.camParamsToIntrinsic(
			fx=self.SID/self.pixel_size[0],
			fy=self.SID/self.pixel_size[1],
			image_dims=self.image_size
		)

class ImageCollection(object):
	def __init__(self):
		self.image_set = {}
	
	def add_image(self, image_name, image_data):
		self.image_set[image_name] = image_data
	
	def save_pngs(self, path, root_filename):
		for image_name, image_data in self.image_set.items():
			rgb_image = np.dstack([image_data] * 3)
			im = Image.fromarray(rgb_image.astype(np.uint8))
			im.save(path / f"{root_filename}_{image_name}.png")

	def save(self, path, filename, save_npz=False, save_png=False, compress=True):
		if save_npz:
			if compress:
				np.savez_compressed(path / filename, **self.image_set)
			else:
				np.savez(path / filename, **self.image_set)
		else:
			np.save(path / filename, np.dstack(list(self.image_set.values())))
		
		if save_png:
			self.save_pngs(path, filename)

class XRayRenderer(_Renderer):
	def __init__(self, SID=1040, beam_energy=150, pixel_size=(0.25, 0.25), image_size=(1526, 1496), debug=False):
		super().__init__(SID, beam_energy, pixel_size, image_size)
		self.debug = debug
		if not self.debug:
			self._redirect_output()

		try:
			self.gvxr = importlib.import_module('gvxrPython3')
		except ImportError:
			raise ImportError("GVXR has not been installed")

		self._init_gvxr()

	def __del__(self):
		self.gvxr.removePolygonMeshesFromSceneGraph()

		if not self.debug:
			self._restore_output()

	def _init_gvxr(self):
		importlib.reload(self.gvxr)
		self.gvxr.createWindow(0, 1, "EGL")

		# NOTE: To obtain a like-for-like rendering wrt Optical render,
		# 		replace below lines with the following and apply a horizontal flip to the 
		#		result
		#
		# 		self.gvxr.setSourcePosition(0.0, -0.5*self.SID, 0.0, "mm")
		# 		self.gvxr.setDetectorPosition(0.0,0.5*self.SID, 0.0, "mm")

		self.gvxr.setSourcePosition(0.0, 0.5*self.SID, 0.0, "mm")
		self.gvxr.setDetectorPosition(0.0, -0.5*self.SID, 0.0, "mm")
		self.gvxr.usePointSource()
		self.gvxr.setMonoChromatic(int(self.beam_energy), "keV", 1000)
		self.gvxr.setDetectorUpVector(0, 0, -1)
		self.gvxr.setDetectorNumberOfPixels(self.image_size[0], self.image_size[1])
		self.gvxr.setDetectorPixelSize(self.pixel_size[0], self.pixel_size[1], "mm")
		self.gvxr.disableArtefactFiltering()

	def _redirect_output(self):
		pass
		# self._devnull = open(os.devnull, 'w')

		# self._orig_stdout_fno = os.dup(sys.stdout.fileno())
		# self._orig_stderr_fno = os.dup(sys.stderr.fileno())
		# os.dup2(self._devnull.fileno(), 1)
		# os.dup2(self._devnull.fileno(), 2)

	def _restore_output(self):
		os.dup2(self._orig_stdout_fno, 1)  # restore stdout
		os.dup2(self._orig_stderr_fno, 2)  # restore stderr
		self._devnull.close()

	def get_image(self, **kwargs):
		image = np.array(self.gvxr.computeXRayImage())
		depth = np.array(self.gvxr.computeLBuffer("Exported"))
	
		return image.astype(np.uint8), depth

	def centre_table(self):
		self.gvxr.moveToCenter("Exported")

	def orient_fluoroscope(self, new_ppa_angle=0, new_psa_angle=0):
		old_ppa_angle = self.configuration["angle"][0]
		old_psa_angle = self.configuration["angle"][1]

		self.configuration["angle"][0] = new_ppa_angle
		self.configuration["angle"][1] = new_psa_angle

		# Rotation is not commutative, so first undo any existing move in reverse order 
		self.gvxr.rotateNode("Exported", -old_psa_angle, 1, 0, 0)
		self.gvxr.rotateNode("Exported", -old_ppa_angle, 0, 0, 1)

		self.gvxr.rotateNode("Exported", new_ppa_angle, 0, 0, 1)
		self.gvxr.rotateNode("Exported", new_psa_angle, 1, 0, 0)

	def move_table(self, new_table_x=0, new_table_y=0, new_table_z=0):
		old_table_x = self.configuration["position"][0]
		old_table_y = self.configuration["position"][1]
		old_table_z = self.configuration["position"][2]

		self.configuration["position"][0] = new_table_x
		self.configuration["position"][1] = new_table_y
		self.configuration["position"][2] = new_table_z

		delta_table_x = new_table_x - old_table_x
		delta_table_y = new_table_y - old_table_y
		delta_table_z = new_table_z - old_table_z

		self.gvxr.translateScene(delta_table_x, delta_table_y, delta_table_z, "mm")

	def add_mesh(self, stl_filepath, element="I"):
		# Note that the below does not work if the file is a Scene rather than a Mesh
		self.gvxr.loadMeshFile("Exported", stl_filepath, "mm")
		self.gvxr.setElement("Exported", element)

	def deformMesh(self, xx=1, yy=1, zz=1, yx=0, zx=0, xy=0, zy=0, xz=0, yz=0):
		self.gvxr.shearNode("Exported", yx, zx, xy, zy, xz, yz)
		self.gvxr.scaleNode("Exported", xx, yy, zz, "mm")
	
	def resetMesh(self):
		self.gvxr.setNodeTransformationMatrix("Exported", [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
	
	def save_image_set(self, angles_sequence, save_dir, view_name, file_suffix="", save_png=False, save_npz=True, compress=True, resize_to=None):
		self.centre_table()

		images_output = ImageCollection()
		print(angles_sequence)
		for i, (ppa, psa) in enumerate(angles_sequence):
			self.orient_fluoroscope(ppa, psa)
			image, _ = self.get_image()

			if resize_to is not None:
				image = self.resize_image(image, resize_to=resize_to)

			images_output.add_image(f"{ppa:.1f}_{psa:.1f}", image)

		images_output.save(save_dir, f"{view_name}{file_suffix}", save_npz=save_npz, save_png=save_png, compress=compress)

		return True
	
	@staticmethod
	def resize_image(image, resize_to, order=3):
		assert isinstance(resize_to, list) and len(resize_to) == 2, "Resize must be specified as [width, height]"
		new_width, new_height = resize_to

		min_before = image[np.nonzero(image)].min()
		image_max_before = image.max()
		image = sk_resize(image, (new_height, new_width), order=order)
		image_max_after = image.max()
		image *= image_max_before/image_max_after

		image[image < min_before] = 0

		return image

class RenderEngine:
	@staticmethod
	def make_images(folder_path, image_set_definitions, file_suffix="", camera_config=None, debug=False, overwrite=False, save_png=False, save_npz=True, compress=True, resize_to=None):
		print(f"\nMaking images for {folder_path}")
		file_extension = "npz" if save_npz else "npy"
		if not overwrite and all([(folder_path / f"{name}{file_suffix}.{file_extension}").exists() for name in image_set_definitions]):
			return True
		
		t1 = time.time()
		r = XRayRenderer(debug=debug)
		t2 = time.time()

		meshes_and_their_chemical_elements = {
			"mesh.stl": "I",
			# "myo_lv.stl": "I",
			# "myo_rv.stl": "I",
			# "myo_epi.stl": "I",
			# "joined.stl": "I"
		}

		t3 = time.time()
		for mesh_filepath, element in meshes_and_their_chemical_elements.items():
			r.add_mesh(str(folder_path / mesh_filepath), element)
		
		t4 = time.time()
		for set_name, set_angles in image_set_definitions.items():
			r.save_image_set(set_angles, folder_path, view_name=set_name, file_suffix=file_suffix, resize_to=resize_to, save_png=save_png, save_npz=save_npz, compress=compress)

		t5 = time.time()

		print(f"Time to load scene graph: {t2-t1}")
		print(f"Time to add meshes: {t4-t3}")
		print(f"Time to save images: {t5-t4}")


		return True

	@classmethod
	def generate_data(cls, 
			mesh_dirs, 
			image_set_definitions,
			file_suffix="",
			n_processes=-1,
			camera_config=None,
			debug=False, 
			overwrite=False, 
			save_png=False, 
			save_npz=True, 
			compress=True,
			resize_to=None,
			job_id=None, 
			active_jobs=None,
		):
		if n_processes < 0:
			n_processes = multiprocessing.cpu_count()
		
		start_time = time.time()
		pool = multiprocessing.Pool(processes=n_processes)

		successful = []
		unsuccessful = []

		def add_successful(result):
			successful.append(result)
			if active_jobs is not None and job_id is not None:
				active_jobs.report_item_completed(job_id)

		def add_unsuccessful(result):
			unsuccessful.append(result)
			if active_jobs is not None and job_id is not None:
				active_jobs.report_item_failed(job_id)

		print(f"\nGenerating images for {len(mesh_dirs)} meshes")
		jobs = [
				pool.apply_async(
					cls.make_images, 
					kwds={
						"folder_path": path, 
						"image_set_definitions": image_set_definitions, 
						"file_suffix": file_suffix,
						"camera_config": camera_config,
						"debug": debug, 
						"overwrite": overwrite, 
						"save_png": save_png, 
						"save_npz": save_npz,
						"compress": compress,
						"resize_to": resize_to
					}, 
					callback=add_successful, 
					error_callback=add_unsuccessful
				)
				for path 
				in mesh_dirs
			]

		pool.close()
		result_list_tqdm = []
		for job in tqdm(jobs):
			result_list_tqdm.append(job.get())
		pool.join()

		elapsed_time = time.time() - start_time

		return successful, unsuccessful, elapsed_time

