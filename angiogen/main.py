from pathlib import Path
from entities.render import RenderEngine

root_dir = Path("/data")

meshes = sorted(root_dir.rglob("**/mesh.stl"))
mesh_dirs = [mesh.parents[0] for mesh in meshes]

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

# successful, unsuccessful, elapsed_time = RenderEngine.generate_data(
# 	mesh_dirs, 
# 	image_set_definitions,
# 	**args
# )

RenderEngine.make_images(**{
	"folder_path": mesh_dirs[0], 
	"image_set_definitions": image_set_definitions, 
	"file_suffix": None,
	"camera_config": None
}, **args)

# if len(unsuccessful) > 0:
# 	print("ALERT: Some jobs finished unsuccessfully.")
# 	for item in unsuccessful:
# 		print(item)
# elif len(successful) == 50:
# 	print("All jobs finished successfully.")

# print(f"--- {elapsed_time} seconds ---")