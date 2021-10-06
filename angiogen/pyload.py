from multiprocessing import Pool
import multiprocessing.pool as mpp

# Sample flask API for docker demo
from flask_restful import Resource, Api
from flask import Flask, send_file, jsonify

from pathlib import Path
from base64 import encodebytes

import os
import sys
import base64
#import pyrender
import importlib
import numpy as np
from PIL import Image
from io import BytesIO

import importlib

from MatrixCalculator import MatrixCalculator

# This function is purely to allow a tqdm progress bar
def istarmap(self, func, iterable, chunksize=1):
	"""starmap-version of imap
	"""
	self._check_running()
	if chunksize < 1:
		raise ValueError(
			"Chunksize must be 1+, not {0:n}".format(
				chunksize))

	task_batches = mpp.Pool._get_tasks(func, iterable, chunksize)
	result = mpp.IMapIterator(self)
	self._taskqueue.put(
		(
			self._guarded_task_generation(result._job,
										  mpp.starmapstar,
										  task_batches),
			result._set_length
		))
	return (item for chunk in result for item in chunk)

class _Renderer(object):
	def __init__(self):
		self.configuration = {
			"angle": [0, 0],        #(Positioner Primary Angle, Positioner Secondary Angle)
			"position": [0, 0, 0]   #(TableX, TableY, TableZ)
		}

	def load_mesh(self, path):
		import pyrender
		dat = np.load(path)

		verts = dat["verts"]
		faces = dat["faces"]

		return pyrender.Mesh([pyrender.Primitive(positions=verts, indices=faces)])

	@property
	def SID(self):
		return 1040

	@property
	def beam_energy(self):
		return 150

	@property
	def pixel_size(self):
		return (
			0.25,0.25
		)

	@property
	def image_size(self):
		return (
			1526,
			1496
		)

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

	def perform_protocol(self, protocol,**kwargs):
		images = []
		depths = []
		matrices = {"K": [], "P": [], "R": [], "t": []}

		for item in protocol:
			if "capture" in item:
				im, dp = self.get_image(**kwargs)

				images += [im]
				depths += [dp]

				mats = MatrixCalculator.poseToExtrinsics(self.pose)
				matrices["K"] += [self.cameraMatrix]
				matrices["P"] += [self.pose]
				matrices["R"] += [mats["R"]]
				matrices["t"] += [mats["t"]]

			if "centre" in item:
				self.centre_table(protocol_item=item, **kwargs)
			
			if "fluoroscope" in item:
				self.orient_fluoroscope(protocol_item=item, **kwargs)

			if "table" in item:
				self.move_table(protocol_item=item, **kwargs)

		for matrix in matrices:
			matrices[matrix] = np.dstack(matrices[matrix])

		return np.dstack(images), np.dstack(depths), matrices

	def generate_data(self, mesh):
		raise NotImplementedError("Renderer must implement a `generate_data` method")

	def get_image(self, **kwargs):
		raise NotImplementedError("Renderer must implement a `get_image` method")

	def centre_table(self, **kwargs):
		raise NotImplementedError("Renderer must implement a `centre_table` method")

	def orient_fluoroscope(self, **kwargs):
		raise NotImplementedError("Renderer must implement a `orient_fluoroscope` method")

	def move_table(self, **kwargs):
		raise NotImplementedError("Renderer must implement a `move_table` method")

class OpticalRenderer(_Renderer):
	def __init__(self, cfg):
		super().__init__()
		self.config = cfg

		self.scene = None
		self.camera = None
		self.light = None
		self.mesh = None
		self.renderer = None
		self.viewer = None

		self.configuration = {
			"angle": [0, 0],        #(Positioner Primary Angle, Positioner Secondary Angle)
			"position": [0, 0, 0]   #(TableX, TableY, TableZ)
		}
		self._init_pyrender()

	def _init_pyrender(self):
		import pyrender
		self.scene = pyrender.Scene()
		self.light = self.scene.add(pyrender.DirectionalLight(color=np.ones(3), intensity=1.0), self.pose)
		self.renderer = pyrender.OffscreenRenderer(*self.image_size)
		self.camera = self.scene.add(
			pyrender.IntrinsicsCamera(
				fx=self.SID/self.pixel_size[0],
				fy=self.SID/self.pixel_size[1],
				cx=0.5*self.image_size[0],
				cy=0.5*self.image_size[1],
				zfar=1e12 # `Infinite` clipping
			), 
			self.pose
		)

	def get_image(self, **kwargs):
		self.scene.set_pose(self.light, self.pose)
		self.scene.set_pose(self.camera, self.pose)

		# self.viewer = pyrender.Viewer(self.scene, viewport_size=self.image_size)
		image, depth = self.renderer.render(self.scene)

		# Convert to single channel (greyscale)

		greyscale = True
		if greyscale:
			image = np.array(image[:, :, 0])*.299 + np.array(image[:, :, 1])*.587 + np.array(image[:, :, 2])*.114

		return image.astype(np.uint8), depth

	def centre_table(self, mesh, **kwargs):
		cx, cy, cz = mesh.centroid
		self.configuration["position"] = [-cx, -cy, -cz]

	def orient_fluoroscope(self, protocol_item, **kwargs):
		self.configuration["angle"][0] = -protocol_item["fluoroscope"].get("ppa", self.configuration["angle"][0])
		self.configuration["angle"][1] = protocol_item["fluoroscope"].get("psa", self.configuration["angle"][1])

	def move_table(self, protocol_item, **kwargs):
		self.configuration["position"][0] = protocol_item["table"].get("x", self.configuration["position"][0])
		self.configuration["position"][1] = protocol_item["table"].get("y", self.configuration["position"][1])
		self.configuration["position"][2] = protocol_item["table"].get("z", self.configuration["position"][2])

	def generate_data(self, mesh, **kwargs):
		mesh_obj = self.scene.add(mesh)

		protocol = [["capture"]]

		images, depths, matrices = self.perform_protocol(protocol, mesh=mesh)

		self.scene.remove_node(mesh_obj)

		return images, depths, matrices

class XRayRenderer(_Renderer):
	def __init__(self, cfg, debug=False):
		super().__init__()
		self.config = cfg
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
		
		# self.gvxr.destroyWindow(1)
	# 	self.gvxr.destroyAllWindows()

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

	def centre_table(self, mesh, **kwargs):
		cx, cy, cz = mesh.centroid
		self.configuration["position"] = [-cx, -cy, -cz]

		self.gvxr.moveToCenter("Exported")

	def orient_fluoroscope(self, protocol_item, **kwargs):
		old_ppa_angle = self.configuration["angle"][0]
		old_psa_angle = self.configuration["angle"][1]

		new_ppa_angle = -protocol_item["fluoroscope"].get("ppa", self.configuration["angle"][0])
		new_psa_angle = protocol_item["fluoroscope"].get("psa", self.configuration["angle"][1])

		self.configuration["angle"][0] = new_ppa_angle
		self.configuration["angle"][1] = new_psa_angle

		# Rotation is not commutative, so first undo any existing move in reverse order 
		self.gvxr.rotateNode("Exported", -old_psa_angle, 1, 0, 0)
		self.gvxr.rotateNode("Exported", -old_ppa_angle, 0, 0, 1)

		self.gvxr.rotateNode("Exported", new_ppa_angle, 0, 0, 1)
		self.gvxr.rotateNode("Exported", new_psa_angle, 1, 0, 0)

		# self.gvxr.displayScene()
		# self.gvxr.renderLoop()

	def move_table(self, protocol_item, **kwargs):
		old_table_x = self.configuration["position"][0]
		old_table_y = self.configuration["position"][1]
		old_table_z = self.configuration["position"][2]

		new_table_x = protocol_item["table"].get("x", self.configuration["position"][0])
		new_table_y = protocol_item["table"].get("y", self.configuration["position"][1])
		new_table_z = protocol_item["table"].get("z", self.configuration["position"][2])

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

	def generate_data(self, **kwargs):
		print("Setting props")

		protocol = protocol = [["capture"]]

		images, depths, matrices = self.perform_protocol(protocol, mesh=None)

		return images, depths, matrices
	
	def deformMesh(self, xx=1, yy=1, zz=1, yx=0, zx=0, xy=0, zy=0, xz=0, yz=0):
		self.gvxr.shearNode("Exported", yx, zx, xy, zy, xz, yz)
		self.gvxr.scaleNode("Exported", xx, yy, zz, "mm")
	
	def resetMesh(self):
		self.gvxr.setNodeTransformationMatrix("Exported", [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])

app = Flask(__name__)
api = Api(app)

class OpticalRender(Resource):
	@staticmethod
	def convert_to_bytes_list(image_stack_np):
		result = []
		
		for i in range(image_stack_np.shape[2]):
			image = image_stack_np[:,:,i]

			rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
			rgb_image[:, :, 0] = image
			rgb_image[:, :, 1] = image
			rgb_image[:, :, 2] = image

			im = Image.fromarray(rgb_image.astype(np.uint8))

			img_io = BytesIO()
			im.save(img_io, 'PNG')

			result.append(im)
		
		return result

	def get(self):
		mesh = Path('/data/tt/00001/mesh.npz')

		r = OpticalRenderer(1)
		m = r.load_mesh(mesh)
		images, _, _, = r.generate_data(m)
		# image = images[:,:,0]

		# rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
		# rgb_image[:, :, 0] = image
		# rgb_image[:, :, 1] = image
		# rgb_image[:, :, 2] = image

		# im = Image.fromarray(rgb_image.astype(np.uint8))

		# img_io = BytesIO()
		# im.save(img_io, 'PNG')
		# img_io.seek(0)
		# return send_file(img_io, mimetype='image/png')

		images_bytes_list = self.convert_to_bytes_list(images)
		return jsonify({
			"images": images_bytes_list
		})

def convert_to_bytes_list(image_np):
	
	image = image_stack_np[:,:,i]

	rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
	rgb_image[:, :, 0] = image
	rgb_image[:, :, 1] = image
	rgb_image[:, :, 2] = image

	im = Image.fromarray(rgb_image.astype(np.uint8))

	img_io = BytesIO()
	im.save(img_io, 'PNG')

	return im

def get_xr(mesh_type="pipes_A", mesh_id="00001", n_negatives=32):
	mesh = Path(f'/data/{mesh_type}/{mesh_id}/mesh.npz')
	stl_mesh = f'/data/{mesh_type}/{mesh_id}/mesh.stl'

	r = XRayRenderer(1, debug=True)
	r.add_scene(stl_mesh)

	results = []
	for x in range(1, n_negatives+1):
		r.resetMesh()
		r.deformMesh(xx=2*x, yy=0.5*x, zz=1*x, yx=0.5*x, zx=0.5*x, xy=0.5*x, zy=0.5*x, xz=0.5*x, yz=0.5*x)
		images, _, _, = r.generate_data()
		image = images[:,:,0]

		rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
		rgb_image[:, :, 0] = image
		rgb_image[:, :, 1] = image
		rgb_image[:, :, 2] = image

		im = Image.fromarray(rgb_image.astype(np.uint8))

		img_io = BytesIO()
		im.save(img_io, 'PNG')
		img_io = encodebytes(img_io.getvalue()).decode('ascii')
		
		results.append(img_io)

	return results
	# img_io.seek(0)
	# return send_file(img_io, mimetype='image/png')


def generate_images():
	seed_start, seed_end = 1, 1
	seeds = range(seed_start, seed_end + 1)

	max_processes = 4

	num_processes = len(seeds) if len(seeds) < max_processes else max_processes
	mpp.Pool.istarmap = istarmap

	results = []
	n_negatives = 4

	with Pool(num_processes) as p:
		iterable = [("pipes_B", f"{seed:05}", n_negatives) for seed in seeds]
		for result in p.istarmap(get_xr, iterable):
			results.append(result)
	
	# return results
	return jsonify({
			"images": results
		})

import time

class XRayRender(Resource):
	def get(self):
		t1 = time.time()

		results = generate_images()

		t2 = time.time()
		
		print(t2 - t1)
		return results

api.add_resource(OpticalRender, '/')
api.add_resource(XRayRender, '/xray')

if __name__ == '__main__':
	app.run(host="0.0.0.0", debug=True)