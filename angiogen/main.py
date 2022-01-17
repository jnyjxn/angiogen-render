from pathlib import Path
from entities.render import RenderEngine

root_dir = Path("/data")

meshes = sorted(root_dir.rglob("**/mesh.stl"))
mesh_dirs = [mesh.parents[0] for mesh in meshes]

import math

def generate_equidistant_points(n_points):
	a = 4*math.pi/n_points
	d = math.sqrt(a)

	M_theta = round(math.pi/d)
	d_theta = math.pi / M_theta
	d_phi = a/d_theta

	for m in range(M_theta - 1):
		theta = math.pi*(m + 0.5)/M_theta
		M_phi = round(2*math.pi*math.sin(theta)/d_phi)
		for n in range(M_phi - 1):
			phi = 2*math.pi*(n + 0.5)/M_phi
			theta_d = (theta / math.pi) * 180
			phi_d = (phi / math.pi) * 180
			yield [180 - theta_d, phi_d]

image_set_definitions = [
	{
		"name": "ims",
		"angles": [
			[0,  0],
			[10, 0],
		]
	}
]
args = {
	"resize_to": [256, 256],
	# "n_processes": 2,
	"save_png": True,
	"save_npz": True,
	"compress": True,
	"overwrite": True,
	"debug": True
}

successful, unsuccessful, elapsed_time = RenderEngine.generate_data(
	mesh_dirs, 
	image_set_definitions,
	**args
)

# RenderEngine.make_images(**{
# 	"folder_path": mesh_dirs[0], 
# 	"image_set_definitions": image_set_definitions, 
# 	"file_suffix": None,
# 	"camera_config": None
# }, **args)

if len(unsuccessful) > 0:
	print("ALERT: Some jobs finished unsuccessfully.")
	for item in unsuccessful:
		print(item)
elif len(successful) == 50:
	print("All jobs finished successfully.")

print(f"--- {elapsed_time} seconds ---")