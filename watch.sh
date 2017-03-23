#!/bin/bash

fswatch -0 my_app/src --exclude assets.h | while read -d "" event 
do 
    echo "-------------------------------------------------------------------"
    docker exec kore.io.1 kodev build
    docker exec kore.io.1 kodev reload
done


