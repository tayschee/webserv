#!/bin/bash

IMAGE_NAME=nginx_serv
CONTAINER_NAME=nginx_container
NG_SRCS_PATH=$(pwd)/srcs
VM_SRCS_PATH=/srcs

VM_CONFIG_PATH=/etc/nginx/conf.d/default.conf

NG_ERROR_CONF=$(pwd)/nginx_config/error.conf
NG_INDEX_OFF_CONF=$(pwd)/nginx_config/index_off.conf
NG_MULTIPLE_LOCATION_CONF=$(pwd)/nginx_config/multiple_location.conf
NG_REDIRECT_CONF=$(pwd)/nginx_config/redirect.conf
NG_SAME_ERROR_CONF=$(pwd)/nginx_config/same_error.conf

docker build -t nginx_serv ./$IMAGE_NAME #create image

#change permission for test can't be applied immediatly because they cant be git push else
chmod 000 srcs/private
chmod 000 srcs/secret/mdp.html
chmod 000 srcs/spoiler/mdp.html


<< 'C'
#PERSONAL ERROR PAGE TEST
#-d for run in background
docker run --rm -ti -d -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

#-L follow redirect -i http header in output -I only header
curl -i -X GET 127.0.0.1:80
curl -i -X GET 127.0.0.1:80/inexistant_file
curl -i -X INEXISTANT_METHOD 127.0.0.1:80
curl -i -X GET 127.0.0.1:80/secret/secret.html


docker stop $CONTAINER_NAME

#AUTOINDEX TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_INDEX_OFF_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

curl -i -X GET 127.0.0.1:80/
curl -i -X GET 127.0.0.1:80/html/3.html

docker stop $CONTAINER_NAME

#REDIRECT TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_REDIRECT_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

curl -i -L -X GET 127.0.0.1:80/
curl -i -L -X GET 127.0.0.1:80/html/3.html

docker stop $CONTAINER_NAME

#Error on multiple test
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_SAME_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

curl -i -X GET 127.0.0.1:80/
curl -i -X GET 127.0.0.1:80/ffgsdgsfgsg
curl -i -X GET 127.0.0.1:80/secret/secret.html
curl -i -X UNKNOW_METHOD 127.0.0.1:80/html/3.html

docker stop $CONTAINER_NAME
C

#permission reset
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html