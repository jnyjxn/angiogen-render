import time
import numpy as np

from tqdm import tqdm
import multiprocessing
from pathlib import Path
from typing import Optional, Sequence, Tuple, Dict

from fastapi.staticfiles import StaticFiles
from fastapi.concurrency import run_in_threadpool
from fastapi import FastAPI, BackgroundTasks, WebSocket, WebSocketDisconnect

from pydantic import BaseModel

from entities.render import RenderEngine
from entities.jobs import ActiveJobs

class AngioConfig(BaseModel):
	SID: int
	beam_energy: float
	pixel_size: Tuple[float, float]
	image_size: Tuple[int, int]

class AngleSet(BaseModel):
	name: str
	angles: Sequence[Tuple[float, float]]

class RenderRequest(BaseModel):
	angle_sets: Sequence[AngleSet]
	file_suffix: Optional[str] = ""
	config: Optional[AngioConfig]

active_jobs = ActiveJobs()
app = FastAPI()

async def do_render_request(render_request: RenderRequest, job_id: str):
	root_dir = Path("/data")
	
	meshes = sorted(root_dir.rglob("**/mesh.stl"))
	mesh_dirs = list(set([mesh.parents[0] for mesh in meshes]))

	angle_sets = {}
	for angle_set in render_request.angle_sets:
		angle_sets[angle_set.name] = angle_set.angles

	active_jobs.add_job(job_id, len(mesh_dirs))
	_, _, time_taken = await run_in_threadpool(lambda: RenderEngine.generate_data(mesh_dirs, angle_sets, job_id=job_id, active_jobs=active_jobs, n_processes=12))

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
