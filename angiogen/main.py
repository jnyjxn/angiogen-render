import math
from pathlib import Path
from entities.render import RenderEngine
from utils.geometry import generate_equidistant_points

args = {
	# "resize_to": [256, 256], # [width, height], or None to not resize
	"n_processes": -1, # -1 to use all available
	"save_png": True, # save images as a png
	"save_npz": True, # Save images as a numpy npz file, with the angles of the view as the keys, e.g. "90.0_90.0"
	"compress": True, # Only used if save_npz is True: saves a compressed npz file instead of a regular npz file
	"overwrite": True, # If False, will only save files that don't already exist
	"debug": True, # If False, console output is limited to a progress bar
	"renderer_cfg": {
		"SID": 1040, # The Source-Image Distance of the C-Arm in mm. This is the distance between the pixel array and the x-rays source.
		"image_size": [256, 256], # [width, height], in number of pixels
		"pixel_size": [2, 2], # [width, height], in millimeters, relating to real-world detectors. Larger pixels result in "zoomed out" images
		"beam_energy": 500, # in keV. Higher values will appear brighter but small objects may be saturated
		"meshes_and_their_HU": { # Dictionary of mesh filepaths and their corresponding houndsfield units.
			"rightdom-1.stl": 8000, # Note: Depth map is created from the first mesh in this list only.
			"rightdom_epi.stl": -800, 
			"rightdom_lv.stl": -800, 
			"rightdom_rv.stl": -800, 
		}
	}
}

# image_set_definitions is a key-value set of image set names and their corresponding angles.
# The key will therefore always become the name of an output file. If there are two keys, there will be two output files.
# Each angle is a list of angles, in degrees, to view the object in. Those angles are in [theta, phi] order, 
# 	which corresponds to Primary Positioner Angle and Secondary Positioner Angle in clinical DICOM format.
image_set_definitions = {
	"equidistant": list(generate_equidistant_points(50)),
	"orthogonal": [
		[0, 0],
		[0, 90],
		[90, 0],
	]
}

# If using the Docker container, this will always be /data, and the choice of data folder is set by connecting a volume to the container 
# 	(by using the -v flag in the `docker run` command).
root_dir = Path("/data")

# Choose a single mesh file name that is representative of the meshes you are rendering. This is used to look for folders containing renderable meshes.
# Note that **/*.stl is ill-advised if there are multiple meshes per scene, as each mesh will trigger a separate render.
meshes = sorted(root_dir.rglob("**/rightdom-1.stl")) 
mesh_dirs = [mesh.parents[0] for mesh in meshes]

successful, unsuccessful, elapsed_time = RenderEngine.generate_data(
	mesh_dirs, 
	image_set_definitions,
	**args
)

if len(unsuccessful) > 0:
	print("ALERT: Some jobs finished unsuccessfully.")
	for item in unsuccessful:
		print(item)
elif len(successful) == 50:
	print("All jobs finished successfully.")

print(f"--- {elapsed_time} seconds ---")