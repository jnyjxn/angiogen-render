CONTAINER_NAME=flask
sudo docker build -t $CONTAINER_NAME .
xhost +local:`sudo docker inspect --format='{{ .ContainerConfig.Hostname }}' $CONTAINER_NAME`
sudo nvidia-docker run --gpus all -p 5000:5000 -v /data4/jja17/datasets/sparse_geometry:/data -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -it $CONTAINER_NAME:latest