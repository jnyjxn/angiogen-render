from typing import Optional

from fastapi import WebSocket

class Job:
	def __init__(self, job_id: str, total: int):
		self.job_id = job_id
		self.status = "running"
		self.progress = 0
		self.failed = 0
		self.total = total
		self.time_taken = None

		self.subscribers = set()
	
	def report_item_completed(self):
		self.progress += 1
	
	def report_item_failed(self):
		self.failed += 1
	
	def report_job_completed(self, time_taken=None):
		self.status = "completed"
		self.time_taken = time_taken
	
	def add_subscriber(self, subscriber):
		self.subscribers.add(subscriber)
	
	def remove_subscriber(self, subscriber):
		self.subscribers.remove(subscriber)
	
	def remove_all_subscribers(self):
		subs = list(self.subscribers)
		self.subscribers.clear()
		return subs
	
	def to_jsonifiable(self):
		return {
			"job_id": self.job_id,
			"status": self.status,
			"progress": self.progress,
			"failed": self.failed,
			"total": self.total,
			"time": self.time_taken
		}

	async def notify_subscribers(self, subscriber: Optional[WebSocket] = None):
		if subscriber is not None:
			await subscriber.send_json(self.to_jsonifiable())
		else:
			for subscriber in self.subscribers:
				await subscriber.send_json(self.to_jsonifiable())

class ActiveJobs:
	def __init__(self):
		self.jobs = {}
		self.sockets = {}

	def add_job(self, job_id: str, total: int):
		self.jobs[job_id] = Job(job_id, total)
	
	def get_job(self, job_id: str):
		if job_id not in self.jobs:
			return
		
		return self.jobs[job_id].to_jsonifiable()
	
	def report_item_completed(self, job_id: str):
		if job_id not in self.jobs:
			return
		
		self.jobs[job_id].report_item_completed()
	
	async def report_item_completed_and_notify(self, job_id: str):
		if job_id not in self.jobs:
			return

		self.report_item_completed(job_id)
		await self.jobs[job_id].notify_subscribers()
	
	def report_item_failed(self, job_id: str):
		if job_id not in self.jobs:
			return

		self.jobs[job_id].report_item_failed()
	
	async def report_item_failed_and_notify(self, job_id: str):
		if job_id not in self.jobs:
			return
			
		self.report_item_failed(job_id)
		await self.jobs[job_id].notify_subscribers()

	def report_job_completed(self, job_id: str, time_taken: Optional[float]=None):
		if job_id not in self.jobs:
			return
			
		self.jobs[job_id].report_job_completed(time_taken)
	
	async def report_job_completed_and_notify(self, job_id: str, time_taken: Optional[float]=None, remove_all_subscribers: Optional[bool]=True):
		if job_id not in self.jobs:
			return
		
		self.report_job_completed(job_id, time_taken)
		await self.jobs[job_id].notify_subscribers()

		if remove_all_subscribers:
			self.remove_all_subscribers(job_id)

	def add_subscriber(self, job_id: str, subscriber: WebSocket):
		if job_id not in self.jobs:
			return
			
		self.jobs[job_id].add_subscriber(subscriber)
		if subscriber not in self.sockets:
			self.sockets[subscriber] = [job_id]
		else:
			self.sockets[subscriber].append(job_id)

	async def add_subscriber_and_notify(self, job_id: str, subscriber: WebSocket):
		if job_id not in self.jobs:
			return
			
		self.add_subscriber(job_id, subscriber)
		
		await self.jobs[job_id].notify_subscribers(subscriber)
	
	def remove_subscriber(self, subscriber: WebSocket):
		if subscriber not in self.sockets:
			return

		job_ids = self.sockets.pop(subscriber)
		for job_id in job_ids:
			if job_id not in self.jobs:
				continue
				
			self.jobs[job_id].remove_subscriber(subscriber)
	
	def remove_all_subscribers(self, job_id: str):
		if job_id not in self.jobs:
			return
		
		removed_subscribers = self.jobs[job_id].remove_all_subscribers()
		for subscriber in removed_subscribers:
			try:
				self.sockets[subscriber].remove(job_id)
			except:
				pass
			
			if len(self.sockets[subscriber]) == 0:
				self.remove_subscriber(subscriber)
		
