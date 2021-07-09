#!/bin/bash

IMAGE_NAME=nginx_serv
CONTAINER_NAME=nginx_container
NG_SRCS_PATH=$(pwd)/srcs
VM_SRCS_PATH=/srcs

VM_CONFIG_PATH=/etc/nginx/conf.d/default.conf
VM_CONFIG_PATH_DIR=/etc/nginx/conf.d/

NG_ERROR_CONF=$(pwd)/nginx_config/error.conf
NG_INDEX_OFF_CONF=$(pwd)/nginx_config/index_off.conf
NG_MULTIPLE_LOCATION_CONF=$(pwd)/nginx_config/multiple_location.conf
NG_REDIRECT_CONF=$(pwd)/nginx_config/redirect.conf
NG_SAME_ERROR_CONF=$(pwd)/nginx_config/same_error.conf
NG_PHP_ERROR_CONF=$(pwd)/nginx_config/same_error_php.conf
NG_MULTIPLE_CONF=$(pwd)/nginx_config/multiple_config/

#test "name_of_config" "METHOD" "path_to_test" "to add param"
test ()
{
    echo -e "----------------------" $1 " " $2 " " $3 "------------------------\n" >> output
    curl -i -X $2 $4 127.0.0.1:80$3 >> output
}

docker build -t nginx_serv ./$IMAGE_NAME #create image
echo "" > output; #free output
echo ""  >error; #free output

#change permission for test can't be applied immediatly because they cant be git push else
chmod 000 srcs/private
chmod 000 srcs/secret/mdp.html
chmod 000 srcs/spoiler/mdp.html

<< C
#PERSONAL ERROR PAGE TEST
#-d for run in background
docker run --rm -ti -d -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=error.conf 

#-L follow redirect -i http header in output -I only header
test $NAME_CONFIG GET "/"
test $NAME_CONFIG GET "/inexistant_file"
test $NAME_CONFIG NO_METHOD "/"
test $NAME_CONFIG GET "/secret/secret.html"


docker stop $CONTAINER_NAME

#AUTOINDEX OFF TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_INDEX_OFF_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=index_off.conf

test $NAME_CONFIG GET "/"
test $NAME_CONFIG GET "/html/3.html"

docker stop $CONTAINER_NAME

C
#REDIRECT TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_REDIRECT_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=redirect.conf

test $NAME_CONFIG GET "/" "-L"
test $NAME_CONFIG GET "/html/3.html" "-L"


docker stop $CONTAINER_NAME
<< C
#MULTIPLE ERROR TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_SAME_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=same_error.conf

test $NAME_CONFIG GET "/"
test $NAME_CONFIG GET "/ffgsdgsfgsg"
test $NAME_CONFIG GET "/secret/secret.html"
test $NAME_CONFIG NO_METHOD "/html/3.html"

docker stop $CONTAINER_NAME


#MULTIPLE LOCATION TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_LOCATION_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=multiple_location.conf

test $NAME_CONFIG GET "/gif/"
test $NAME_CONFIG GET "/html/"
test $NAME_CONFIG GET "/jpeg/"
test $NAME_CONFIG GET "/png/"
test $NAME_CONFIG GET "/private/"
test $NAME_CONFIG GET "/secret/"
test $NAME_CONFIG GET "/spoiler/"
test $NAME_CONFIG GET "/error/"
test $NAME_CONFIG GET "/php/"
test $NAME_CONFIG NO_METHOD3 "/html/3.html"
test $NAME_CONFIG GET "/unexist.html"

docker stop $CONTAINER_NAME


#PHP ERROR TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_LOCATION_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=same_error_php.conf

test $NAME_CONFIG GET "/fdgfs" #404
test $NAME_CONFIG GET "/private/" #403
test $NAME_CONFIG GET "/secret/" #401
test $NAME_CONFIG GET "/html/cat.html" #405

#MULTIPLE SERVER NAME TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_CONF:$VM_CONFIG_PATH_DIR -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

test "default" GET "/"
test "gif.conf" GET "/" "--header \"Host: gif\""
test "jpeg.conf" GET "/" "--header \"Host: jpeg\""
test "secret.conf" GET "/" "--header \"Host: secret\""



docker stop $CONTAINER_NAME
C
#permission reset
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html