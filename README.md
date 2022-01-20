# AngioRender
An software for generating ML-scale synthetic X-ray datasets from meshes.

## How to use
1. Install `docker` and `nvidia-docker` to use the GPU (this code is not tested on CPU-only machines).
2. Change the `DATA_FOLDER` environment variable in `run.sh`.
3. On UNIX machines, run `sudo sh run.sh`.