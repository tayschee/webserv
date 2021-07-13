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
   test_method $1 GET $2 $3 $4
   test_method $1 HEAD $2 $3 $4
   #test_method $1 OPTIONS $2 $3 $4
   #test_method $1 POST $2 $3 $4
}

test_method ()
{
    echo -e "----------------------" $1 " " $2 " " $3 "------------------------\n" >> output
    curl -i -X $2 $4 127.0.0.1:80$3 >> output
}

#test "name_of_config" "METHOD" "path_to_test" "put param" "get and put param"
test_put()
{
    echo -e "----------------------" $1 " " PUT " " $2 "------------------------\n" >> output
    curl -i -X PUT $4 $3 127.0.0.1:80$2 >> output
    curl -i -X PUT $4 $3 127.0.0.1:80$2 >> output
    curl -i -X GET $4 127.0.0.1:80$2 >> output
    rm -f .$2
}

test_delete()
{
    echo -e "----------------------" $1 " " DELETE " " $2 "------------------------\n" >> output
    curl -i -X GET 127.0.0.1:80$2 >> output
    curl -i -X DELETE 127.0.0.1:80$2 >> output
	curl -i -X DELETE 127.0.0.1:80$2 >> output
    curl -i -X GET 127.0.0.1:80$2 >> output
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

<< PUT_DELETE
#need other tester cant create and delete 2 times the same file
#test_put $NAME_CONFIG "/new.html" "-d <p>impossible</p>"
#test_put $NAME_CONFIG "/private/impossible.html" "-d <p>Ce fichier ne devrait pas pouvoir etre créé</p>"
#test_put_delete $NAME_CONFIG "secret/to_delete.html" "-d <p>Ce fichier devrait pouvoir etre cree apres une demande de mot de passe</p>"
#test_put $NAME_CONFIG "/no_path/new.html" "-d <p>devrait genere une erreur</p>"
#test_put $NAME_CONFIG "/html/new.html" "-d <p>Ne devrait pas marcher</p>"
#test_put_delete $NAME_CONFIG "/" "-d <p>pas de souci</p>" #je connais pas le comportement
#test_put $NAME_CONFIG "/put_test/page.html" "-d <p>PUT : OK</p>"
#essayé avec un body tres gros

cp -r srcs/dir_to_copy srcs/dir_to_delete
test_delete $NAME_CONFIG "/dir_to_delete/cat_symbolic.html"
test_method $NAME_CONFIG GET "/dir_to_delete/cat.html" #test if symbolic link or file which are delete

test_delete $NAME_CONFIG "/dir_to_delete/1.html"
test_delete $NAME_CONFIG "/dir_to_delete/php/"
test_delete $NAME_CONFIG "/dir_to_delete/empty_dir/"
test_delete $NAME_CONFIG "/dir_to_delete/empty_dir/unexist" #unexist
test_delete $NAME_CONFIG "/dir_to_delete/"

test_delete $NAME_CONFIG "/private/"
#test_delete $NAME_CONFIG "/secret/to_delete.html"

PUT_DELETE

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
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html