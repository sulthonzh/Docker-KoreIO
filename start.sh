#!/bin/bash

DIR=$(pwd)

docker stop kore.io.1 2&> /dev/null
docker rm kore.io.1 2&> /dev/null
docker build -t kore.io . 
docker run -d \
    -p 80:8888 \
    --name kore.io.1 \
    -v "${DIR}/my_app":/my_app kore.io 

open http://127.0.0.1

docker logs -f kore.io.1


