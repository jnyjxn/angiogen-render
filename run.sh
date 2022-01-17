CONTAINER_NAME=angiorender
DATA_FOLDER=/data4/jja17/datasets/spheres_test

sudo xhost +local:root
sudo docker build -t $CONTAINER_NAME . && \
sudo docker run \
    --rm \
    --gpus all \
    -p 80:80 \
    -v $DATA_FOLDER:/data \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    -it $CONTAINER_NAME:latest
    # --runtime=nvidia -e NVIDIA_DRIVER_CAPABILITIES=compute,utility -e NVIDIA_VISIBLE_DEVICES=all \