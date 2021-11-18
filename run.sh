CONTAINER_NAME=angiorender
DATA_FOLDER=/data4/jja17/datasets/sparse_geometry

sudo docker build -t $CONTAINER_NAME .
sudo docker run --rm --gpus all -p 5000:5000 -v $DATA_FOLDER:/data -it $CONTAINER_NAME:latest