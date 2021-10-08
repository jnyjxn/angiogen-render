CONTAINER_NAME=flask
sudo docker build -t $CONTAINER_NAME .
sudo docker run --rm -p 5000:5000 -v /data4/jja17/datasets/sparse_geometry:/data -it $CONTAINER_NAME:latest