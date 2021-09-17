# Sample flask API for docker demo
from flask import Flask, send_file
from flask_restful import Resource, Api

from pathlib import Path

import base64
import pyrender
import importlib
import numpy as np
from PIL import Image
from io import BytesIO

import importlib

from MatrixCalculator import MatrixCalculator

class _Renderer(object):
	def __init__(self):
		self.configuration = {
			"angle": [0, 0],        #(Positioner Primary Angle, Positioner Secondary Angle)
			"position": [0, 0, 0]   #(TableX, TableY, TableZ)
		}

	def load_mesh(self, path):
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

app = Flask(__name__)
api = Api(app)

class HelloPythonWithDocker(Resource):
    def get(self):
        mesh = Path('/data/pipes_A/00001/mesh.npz')

        r = OpticalRenderer(1)
        m = r.load_mesh(mesh)
        images, _, _, = r.generate_data(m)
        image = images[:,:,0]

        rgb_image = np.zeros((image.shape[0], image.shape[1], 3))
        rgb_image[:, :, 0] = image
        rgb_image[:, :, 1] = image
        rgb_image[:, :, 2] = image

        im = Image.fromarray(rgb_image.astype(np.uint8))

        img_io = BytesIO()
        im.save(img_io, 'PNG')
        img_io.seek(0)
        return send_file(img_io, mimetype='image/png')
    
api.add_resource(HelloPythonWithDocker, '/')

if __name__ == '__main__':
    app.run(host="0.0.0.0", debug=True)