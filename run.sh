# Change this line to point your data folder
DATA_FOLDER=/data4/jja17/datasets/jacks_set

# Leave below as is
CONTAINER_NAME=angiorender

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