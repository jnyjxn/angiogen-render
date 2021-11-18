import time

import pyrender
import importlib
import numpy as np
from tqdm import tqdm
from PIL import Image
import multiprocessing
from pathlib import Path
from skimage.transform import resize as sk_resize

from MatrixCalculator import MatrixCalculator

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
		self.gvxr.createWindow()

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
		self._devnull = open(os.devnull, 'w')

		self._orig_stdout_fno = os.dup(sys.stdout.fileno())
		self._orig_stderr_fno = os.dup(sys.stderr.fileno())
		os.dup2(self._devnull.fileno(), 1)
		os.dup2(self._devnull.fileno(), 2)

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

	def add_scene(self, stl_filepath):
		# TODO: Figure out why of the below two lines, one works on some meshes and the other on others
		self.gvxr.loadSceneGraph(stl_filepath, "mm")
		# self.gvxr.loadMeshFile("Exported", stl_filepath, "mm")
		self.gvxr.setElement("Exported", "I")
	
	def deformMesh(self, xx=1, yy=1, zz=1, yx=0, zx=0, xy=0, zy=0, xz=0, yz=0):
		self.gvxr.shearNode("Exported", yx, zx, xy, zy, xz, yz)
		self.gvxr.scaleNode("Exported", xx, yy, zz, "mm")
	
	def resetMesh(self):
		self.gvxr.setNodeTransformationMatrix("Exported", [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
	
	def save_image_set(self, angles_sequence, save_dir, view_name, save_png=False, resize=None):
		self.centre_table()

		new_width = None
		new_height = None
		if resize is not None:
			assert isinstance(resize, list) and len(resize) == 2, "If given, resize must be specified as [width, height]"
			new_width, new_height = resize

		imageset = None
		for i, (ppa, psa) in enumerate(angles_sequence):
			self.orient_fluoroscope(ppa, psa)
			image, _ = self.get_image()

			if resize is not None:
				min_before = image[np.nonzero(image)].min()
				image_max_before = image.max()
				image = sk_resize(image, (new_height, new_width), order=3)
				image_max_after = image.max()
				image *= image_max_before/image_max_after

				image[image < min_before] = 0

			if save_png:
				rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
				rgb_image[:, :, 0] = image
				rgb_image[:, :, 1] = image
				rgb_image[:, :, 2] = image

				im = Image.fromarray(rgb_image.astype(np.uint8))

				im.save(save_dir / f"{view_name}_{ppa}_{psa}.png")

			if imageset is None:
				imageset = np.zeros((image.shape[0], image.shape[1], len(angles_sequence)), dtype=np.uint8)
			
			imageset[:, :, i] = image

		np.save(save_dir / f"{view_name}.npy", imageset)

		return True


def make_images(folder_path, image_set_definitions, seed=1, epoch=1, overwrite=False):
	if not overwrite and (folder_path / "eclectic.npy").exists() and (folder_path / "orthogonal.npy").exists():
		return True

	stl_mesh = str(folder_path / "mesh.stl")
	r = XRayRenderer()
	r.add_scene(stl_mesh)
	
	for set_name, set_angles in image_set_definitions.items():
		r.save_image_set(set_angles, folder_path, view_name=set_name, resize=[256, 256], save_png=True)

	return True

successful = []
unsuccessful = []

def add_successful(result):
	successful.append(result)

def add_unsuccessful(result):
	unsuccessful.append(result)

def make_images(mesh_dirs, image_set_definitions, n_processes=-1):
	if n_processes < 0:
		n_processes = multiprocessing.cpu_count()

	start_time = time.time()
	pool = multiprocessing.Pool(processes=n_processes)

	jobs = [
		pool.apply_async(
			make_images, 
			kwds={"folder_path": path, "seed": idx, "overwrite": True}, 
			callback=add_successful, 
			error_callback=add_unsuccessful
			) for idx, path in enumerate(mesh_dirs)
		]
		
	pool.close()
	result_list_tqdm = []
	for job in tqdm(jobs):
		result_list_tqdm.append(job.get())
	pool.join()

	elapsed_time = time.time() - start_time

	return successful, unsuccessful, elapsed_time

if __name__ == "__main__":
	root_dir = Path("/data")
	
	meshes = sorted(root_dir.rglob("**/mesh.stl"))
	mesh_dirs = [mesh.parents[0] for mesh in meshes]

	seed = 10000
	rng = np.random.default_rng(seed)

	orthogonal_set = [
		(0, 0),
		(0, 90),
		(90, 0)
	]

	random_set = []
	for i in range(3):  
		theta = 360*rng.random()
		phi = 180*np.arccos(2*rng.random()-1.0)/np.pi

		random_set += [(theta, phi)]
	
	image_sets = {
		"orthogonal": orthogonal_set,
		"random": random_set,
	}

	successful, unsuccessful, elapsed_time = make_images(mesh_dirs, image_set_definitions)

	if len(unsuccessful) > 0:
		print("ALERT: Some jobs finished unsuccessfully.")
		for item in unsuccessful:
			print(item)
	elif len(successful) == 50:
		print("All jobs finished successfully.")
	
	print("--- %s seconds ---" % (time.time() - start_time))
