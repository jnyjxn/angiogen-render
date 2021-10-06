CONTAINER_NAME=flask
sudo docker build -t $CONTAINER_NAME .
sudo docker run --rm -p 5000:5000 -v /Users/jonnyjackson/Documents/PhD:/data -it $CONTAINER_NAME:latest