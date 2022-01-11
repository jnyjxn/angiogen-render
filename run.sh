CONTAINER_NAME=angiorender
DATA_FOLDER=/data4/jja17/datasets/alt_angio

sudo xhost +local:root
sudo docker build -t $CONTAINER_NAME . && \
sudo docker run \
    --rm \
    --gpus all \
    -p 80:80 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    -v $DATA_FOLDER:/data \
    -it $CONTAINER_NAME:latest