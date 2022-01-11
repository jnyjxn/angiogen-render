import time
import yaml
import numpy as np

from tqdm import tqdm
import multiprocessing
from pathlib import Path
from typing import Optional, Sequence, Tuple, Dict, Union

from fastapi.staticfiles import StaticFiles
from fastapi.concurrency import run_in_threadpool
from fastapi import FastAPI, BackgroundTasks, WebSocket, WebSocketDisconnect

from pydantic import BaseModel

from entities.render import RenderEngine
from entities.jobs import ActiveJobs

def get_camera_params_from_yaml(yaml_path: Path) -> Dict:
	with open(yaml_path, "r") as f:
		yaml_dict = yaml.load(f, Loader=yaml.SafeLoader)

		for item in ['SID', 'beam_energy', 'pixel_size', 'image_size']:
			if item not in yaml_dict:
				raise Exception(f"Missing {item} in {yaml_path}")

		return yaml_dict

active_jobs = ActiveJobs()
app = FastAPI()

default_camera_params = get_camera_params_from_yaml(Path("config/camera.yaml"))

class AngioConfig(BaseModel):
	SID: Optional[int] = default_camera_params['SID']
	beam_energy: Optional[float] = default_camera_params['beam_energy']
	pixel_size: Optional[Tuple[float, float]] = default_camera_params['pixel_size']
	image_size: Optional[Tuple[int, int]] = default_camera_params['image_size']

class AngleSet(BaseModel):
	name: str
	angles: Sequence[Tuple[float, float]]

class RenderRequest(BaseModel):
	root_folder: Optional[Union[Path, str]] = ""
	angle_sets: Sequence[AngleSet]
	file_suffix: Optional[str] = ""
	debug: Optional[bool] = False
	resize_to: Optional[Sequence[int]] = None
	overwrite: Optional[bool] = False
	save_png: Optional[bool] = False
	save_npz: Optional[bool] = True
	compress: Optional[bool] = True
	n_processes: Optional[int] = 4
	camera_params: Optional[AngioConfig] = None

async def do_render_request(render_request: RenderRequest, job_id: str):
	root_dir = Path(f"/data/{render_request.root_folder}")
	
	meshes = sorted(root_dir.rglob("**/mesh.stl"))
	mesh_dirs = list(set([mesh.parents[0] for mesh in meshes]))

	angle_sets = {}
	for angle_set in render_request.angle_sets:
		angle_sets[angle_set.name] = angle_set.angles

	active_jobs.add_job(job_id, len(mesh_dirs))

	_, _, time_taken = await run_in_threadpool(
		lambda: RenderEngine.generate_data(
			mesh_dirs, 
			angle_sets, 
			file_suffix=render_request.file_suffix,
			n_processes=render_request.n_processes,
			camera_config=render_request.camera_params,
			debug=render_request.debug, 
			overwrite=render_request.overwrite, 
			save_png=render_request.save_png,
			save_npz=render_request.save_npz,
			compress=render_request.compress,
			resize_to=render_request.resize_to,
			job_id=job_id, 
			active_jobs=active_jobs, 
		)
	)

	await active_jobs.report_job_completed_and_notify(job_id, time_taken)

@app.post("/render")
async def do_render(render_request: RenderRequest, background_tasks: BackgroundTasks):
	job_id = "job"

	background_tasks.add_task(do_render_request, render_request, job_id)

	return {
		"status": "ok",
		"config": render_request,
		"job_id": job_id
	}

@app.get("/jobs/{job_id}")
def get_job_info(job_id):
	return active_jobs.get_job(job_id) or {"error": "job not found"}

@app.websocket("/ws/{job_id}")
async def websocket_endpoint(subscriber: WebSocket, job_id: str):
	await subscriber.accept()
	try:
		await active_jobs.add_subscriber_and_notify(job_id, subscriber)
		while True:
			try:
				data = await subscriber.receive_text()
			except Exception as e:
				raise e
	except (WebSocketDisconnect, KeyboardInterrupt):
		active_jobs.remove_subscriber(subscriber)	

app.mount("/", StaticFiles(directory="static", html=True), name="static")
