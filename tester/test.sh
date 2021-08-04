#!/bin/bash

NGINX_IP=127.0.0.1
NGINX_PORT=80

WEBSERV_IP=127.0.0.2
WEBSERV_PORT=80

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
    test_method $1 GET $2 $3 $4
    test_method $1 HEAD $2 $3 $4
}

test_method ()
{
    echo $1
    echo $2
    echo $3
    echo $4
    echo -e "----------------------" $1 " " $2 " " $3 "------------------------\n" >> output
    curl -i -X $2 $4 127.0.0.1:80$3 >> output
}

#test "name_of_config" "METHOD" "path_to_test" "put param" "get and put param"
test_put()
{
    echo $3
    echo -e "----------------------" $1 " " PUT " " $2 "------------------------\n" >> output
    curl -i -X PUT $3 $NGINX_IP:$NGINX_PORT$2 >> output
    echo -e "//////////////////////////////////////////////////////////////////\n" >> output
    curl -i -X PUT $3 $NGINX_IP:$NGINX_PORT$2 >> output
    echo -e "//////////////////////////////////////////////////////////////////\n" >> output
    curl -i -X GET $4 $NGINX_IP:$NGINX_PORT$2 >> output
    rm -f .$2
}

test_delete()
{
    echo -e "----------------------" $1 " " DELETE " " $2 "------------------------\n" >> output
    curl -i -X GET $NGINX_IP:$NGINX_PORT$2 >> output
    echo -e "//////////////////////////////////////////////////////////////////\n" >> output
    curl -i -X DELETE $NGINX_IP:$NGINX_PORT$2 >> output
    echo -e "//////////////////////////////////////////////////////////////////\n" >> output
	curl -i -X DELETE $NGINX_IP:$NGINX_PORT$2 >> output
    echo -e "//////////////////////////////////////////////////////////////////\n" >> output
    curl -i -X GET $NGINX_IP:$NGINX_PORT$2 >> output
}

test_syntax()
{
    echo -e "----------------------" $1 " " SYNTAX_TEST  " " "----------------------------\n" >> output
    python send_request.py $1 $NGINX_IP $NGINX_PORT >> output
}

docker build -t nginx_serv ./$IMAGE_NAME #create image
echo "" > output; #clear output
echo ""  > error; #clear error

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
test $NAME_CONFIG "/"
test $NAME_CONFIG "/inexistant_file"
test $NAME_CONFIG "/"
test $NAME_CONFIG "/secret/secret.html"


docker stop $CONTAINER_NAME

#AUTOINDEX OFF TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_INDEX_OFF_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=index_off.conf

test $NAME_CONFIG "/"
test $NAME_CONFIG "/html/3.html"

docker stop $CONTAINER_NAME


#REDIRECT TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_REDIRECT_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=redirect.conf

test $NAME_CONFIG "/" "-L"
test $NAME_CONFIG "/html/3.html" "-L"


docker stop $CONTAINER_NAME

#MULTIPLE ERROR TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_SAME_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=same_error.conf

test $NAME_CONFIG "/"
test $NAME_CONFIG "/ffgsdgsfgsg"
test $NAME_CONFIG "/secret/secret.html"
test $NAME_CONFIG "/html/3.html"

docker stop $CONTAINER_NAME

C
#MULTIPLE LOCATION TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_LOCATION_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=multiple_location.conf

<< GET
test $NAME_CONFIG "/gif/"
test $NAME_CONFIG "/html/"
test $NAME_CONFIG "/jpeg/"
test $NAME_CONFIG "/png/"
test $NAME_CONFIG "/private/"
test $NAME_CONFIG "/secret/"
test $NAME_CONFIG "/spoiler/"
test $NAME_CONFIG "/error/"
test $NAME_CONFIG "/php/"
test $NAME_CONFIG "/html/3.html"
test $NAME_CONFIG "/unexist.html"
GET

#<< PUT
#need other tester cant create and delete 2 times the same file
#test_put $NAME_CONFIG "/new.html" "-d \"<p>impossible</p>\"" #dont work
#test_put $NAME_CONFIG "/private/impossible.html" "-d \"impossible2\"" #dont work
#test_put $NAME_CONFIG "/secret/to_delete.html" "-d \"<p>secret</p>\"" #must do test
#test_put $NAME_CONFIG "/no_path/new.html" "-d \"<p>error</p>\"" #dont work
#test_put $NAME_CONFIG "/html/new.html" "-d \"<p>NO</p>\"" #work
#"/html/new.html" delete in delete_test
#test_put $NAME_CONFIG "/put_and_delete/page.html" "-d \"<p>OK</p>\"" #work

#test_put $NAME_CONFIG "/" "-d \"<p>work</p>\"" #je connais pas le comportement
#essayé avec un body tres gros
#PUT

cp -r srcs/dir_to_copy srcs/dir_to_delete
#test_delete $NAME_CONFIG "/dir_to_delete/cat_symbolic.html"
#test_method $NAME_CONFIG GET "/dir_to_delete/cat.html" #test if symbolic link or file which are delete

#test_delete $NAME_CONFIG "/dir_to_delete/cat.html"
#test_delete $NAME_CONFIG "/dir_to_delete/1.html"
#test_delete $NAME_CONFIG "/dir_to_delete/php/"
#test_delete $NAME_CONFIG "/dir_to_delete/empty_dir/"
#test_delete $NAME_CONFIG "/dir_to_delete/empty_dir/unexist" #unexist
#test_delete $NAME_CONFIG "/dir_to_delete/"

#test_delete $NAME_CONFIG "/html/new.html" #dont work
rm -f ./srcs/html/new.html
#test_delete $NAME_CONFIG "/private/"
#test_delete $NAME_CONFIG "/secret/to_delete.html" #dont do it

rm -rf srcs/dir_to_delete

#test_syntax syntax_ressources/wrong_uri
#test_syntax syntax_ressources/line_feed
#test_syntax syntax_ressources/line_feed2
#test_syntax syntax_ressources/multiple_space
#test_syntax syntax_ressources/space_and_tab
#test_syntax syntax_ressources/tab

#test_post $NAME_CONFIG /php/1.php
#test_post $NAME_CONFIG /php/2.php
#test_post $NAME_CONFIG /php/exemple.php
#test_post $NAME_CONFIG /php/info.php
#test_method $NAME_CONFIG POST /php/php.php "-d arg1=O -d arg2=K -d arg3=!"
#test_method $NAME_CONFIG POST /php/php.php "-d arg1=ceci -d arg2=EST -d arg3=method -d arg4=POST"
test_method $NAME_CONFIG GET /php/php.php "-G -d arg1=GET -d arg2=query -d arg3=STRING"

docker stop $CONTAINER_NAME

<< C
#PHP ERROR TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_LOCATION_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

NAME_CONFIG=same_error_php.conf

test $NAME_CONFIG "/fdgfs" #404
test $NAME_CONFIG "/private/" #403
test $NAME_CONFIG "/secret/" #401
test $NAME_CONFIG "/html/cat.html" #405

#MULTIPLE SERVER NAME TEST
docker run -d --rm -ti -v $NG_SRCS_PATH:$VM_SRCS_PATH -v $NG_MULTIPLE_CONF:$VM_CONFIG_PATH_DIR -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
sleep 5

test "default" "/"
test "gif.conf" "/" "--header \"Host: gif\""
test "jpeg.conf" "/" "--header \"Host: jpeg\""
test "secret.conf" "/" "--header \"Host: secret\""

docker stop $CONTAINER_NAME
C
#permission reset

#permission
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html