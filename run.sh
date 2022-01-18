CONTAINER_NAME=angiorender
DATA_FOLDER=/data4/jja17/datasets/spheres

sudo xhost +local:root
sudo docker build -t $CONTAINER_NAME . && \
sudo nvidia-docker run \
    --runtime=nvidia \
    --rm \
    --gpus all \
    -p 80:80 \
    -v $DATA_FOLDER:/data \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    -it $CONTAINER_NAME:latest