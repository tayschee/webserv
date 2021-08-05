#!/bin/bash
GREEN='\e[38;5;2'
ORANGE='\e[38;5;208'
RED='\e[38;5;196'
WHITE='\e[39'
#MOST CHANGE VARIABLE
OUTPUT=output #stdin output
ERROR_OUTPUT=/dev/stderr #stderr output

WEBSERV_DIR=..
WEBSERV=./$WEBSERV_DIR/webserv

NGINX_IP=127.0.0.1
NGINX_PORT=80
WEBSERV_IP=127.0.0.1
WEBSERV_PORT=80

NGINX_IMAGE=nginx_serv
PATH_TO_DOCKERFILE=./nginx_serv

SRCS_PATH=$(pwd)/srcs

SLEEP_TIMER=5
#END MOST CHANGE VARIABLE 

NGINX_TMP=nginx_tmpfile #give filename $NGINX_TMP"1" $NGINX_TMP"2" $NGINX_TMP"3" $NGINX_TMP"4" which doesnt exist
WEBSERV_TMP=webserv_tmpfile #same than NGINX_TMP
TMP=tmp #give filename which doesn t exist
SAVE=save
NG_DELETE_DIR=delete_dir1
WS_DELETE_DIR=delete_dir2
IMAGE_NAME=nginx_serv
CONTAINER_NAME=nginx_container
VM_SRCS_PATH=/srcs
VM_CONFIG_PATH_DIR=/etc/nginx/conf.d/
VM_CONFIG_PATH=/etc/nginx/conf.d/default.conf

NG_DIRECTORY_CONF=$(pwd)/nginx_config
WS_DIRECTORY_CONF=$(pwd)/webserv_config

ERROR_CONF=error.conf
INDEX_OFF_CONF=index_off.conf
MULTIPLE_LOCATION_CONF=multiple_location.conf
REDIRECT_CONF=redirect.conf
SAME_ERROR_CONF=same_error.conf
PHP_ERROR_CONF=same_error_php.conf
MULTIPLE_CONF=multiple_config

NG_ERROR_CONF=$NG_DIRECTORY_CONF/$ERROR_CONF
NG_INDEX_OFF_CONF=$NG_DIRECTORY_CONF/$INDEX_OFF_CONF
NG_MULTIPLE_LOCATION_CONF=$NG_DIRECTORY_CONF/$MULTIPLE_LOCATION_CONF
NG_REDIRECT_CONF=$NG_DIRECTORY_CONF/$REDIRECT_CONF
NG_SAME_ERROR_CONF=$NG_DIRECTORY_CONF/$SAME_ERROR_CONF
NG_PHP_ERROR_CONF=$NG_DIRECTORY_CONF/$PHP_ERROR_CONF
NG_MULTIPLE_CONF=$NG_DIRECTORY_CONF/$MULTIPLE_CONF

WS_ERROR_CONF=$WS_DIRECTORY_CONF/$ERROR_CONF
WS_INDEX_OFF_CONF=$WS_DIRECTORY_CONF/$INDEX_OFF_CONF
WS_MULTIPLE_LOCATION_CONF=$WS_DIRECTORY_CONF/$MULTIPLE_LOCATION_CONF
WS_REDIRECT_CONF=$WS_DIRECTORY_CONF/$REDIRECT_CONF
WS_SAME_ERROR_CONF=$WS_DIRECTORY_CONF/$SAME_ERROR_CONF
WS_PHP_ERROR_CONF=$WS_DIRECTORY_CONF/$PHP_ERROR_CONF
WS_MULTIPLE_CONF=$WS_DIRECTORY_CONF/$MULTIPLE_CONF

#test "name_of_config" "METHOD" "path_to_test" "to add param"
launch_server()
{
    docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME >> /dev/null
    if [[ $! != 0 ]]; then
        echo Nginx run
    else
        echo Nginx failed
        exit 1
    fi
    #$WEBSERV $WS_DIRECTORY_CONF/$1 &

    #WEBSERV_PID=$!
    NAME_CONFIG=$1
    sleep $SLEEP_TIMER
}

launch_multi_server()
{
    docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH_DIR -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME >> /dev/null
    if [[ $! != 0 ]]; then
        echo Nginx run
    else
        echo Nginx failed
        exit 1
    fi
    #$WEBSERV $WS_DIRECTORY_CONF/$1 &

    #WEBSERV_PID=$!
    sleep $SLEEP_TIMER
}

stop_server()
{
    docker stop $CONTAINER_NAME
    #kill -2 $WEBSERV_PID #sigint webserv
}

test ()
{
    test_method $1 GET ${@:2}
    test_head $@
}

test_method ()
{
    curl -sSiX $2 ${@:4} $NGINX_IP:$NGINX_PORT$3 >> output2

    #diff <(curl -sSiX $2 ${@:4} $NGINX_IP:$NGINX_PORT$3) <(curl -sSiX $2 ${@:4} $WEBSERV_IP:$WEBSERV_PORT$3) > $TMP
    #if [[ $? != 0 ]]; then
    #    echo -e "----------------------" $1 " " $2 " " $3 "------------------------\n" >> $OUTPUT
    #    cat $TMP >> $OUTPUT
    #fi
    #rm -f $TMP
}

test_head ()
{
    diff <(curl -sSiIX HEAD ${@:3} $NGINX_IP:$NGINX_PORT$2) <(curl -sSiIX HEAD ${@:3} $WEBSERV_IP:$WEBSERV_PORT$2) > $TMP
    if [[ $? != 0 ]]; then
        /bin/echo -e "----------------------" $1 " " HEAD " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        /bin/echo -e $1 " " HEAD " " $2 -e ": NOT OK"
    else
        /bin/echo -e $1 " " HEAD " " $2 ": OK"
    fi
    rm -f $TMP
}

#test "name_of_config" "METHOD" "path_to_test" "put param" "get and put param"
test_put()
{
    curl -i -X PUT "$3" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"1"
    curl -i -X PUT "$3" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"2"
    curl -i -X GET "$4" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"3"
    rm -f ./srcs$2
    curl -i -X PUT "$3" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"1"
    curl -i -X PUT "$3" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"2"
    curl -i -X GET "$4" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"3"

    diff $WEBSERV_TMP"1" $NGINX_TMP"1" > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " PUT1 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    diff $WEBSERV_TMP"2" $NGINX_TMP"2" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " " PUT2 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    diff $WEBSERV_TMP"3" $NGINX_TMP"3" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER PUT " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi

    rm -f $NGINX_TMP"1"
    rm -f $NGINX_TMP"2"
    rm -f $NGINX_TMP"3"
    rm -f $WEBSERV_TMP"1"
    rm -f $WEBSERV_TMP"2"
    rm -f $WEBSERV_TMP"3"
    rm -f $TMP
    rm -f ./srcs$2
}

test_delete()
{
    NG_PATH=$NG_DELETE_DIR$2
    WS_PATH=$WS_DELETE_DIR$2

    curl -i -X GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP"1"
    curl -i -X DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP"2"
	curl -i -X DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP"3"
    curl -i -X GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP"4"

    curl -i -X GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP"1"
    curl -i -X DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP"2"
	curl -i -X DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP"3"
    curl -i -X GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP"4"


    diff $WEBSERV_TMP"1" $NGINX_TMP"1" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET BEFORE DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    diff $WEBSERV_TMP"2" $NGINX_TMP"2" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE 1" $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    diff $WEBSERV_TMP"3" $NGINX_TMP"3" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE2 " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    diff $WEBSERV_TMP"4" $NGINX_TMP"4" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi

    rm -f $NGINX_TMP"1"
    rm -f $NGINX_TMP"2" 
    rm -f $NGINX_TMP"3" 
    rm -f $NGINX_TMP"4" 
    rm -f $WEBSERV_TMP"1"
    rm -f $WEBSERV_TMP"2" 
    rm -f $WEBSERV_TMP"3" 
    rm -f $WEBSERV_TMP"4"

    rm -f $TMP
}

test_syntax()
{
    diff <(python send_request.py $1 $NGINX_IP $NGINX_PORT) <(python send_request.py $1 $WEBSERV_IP $WEBSERV_PORT) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " SYNTAX_TEST  " " "----------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
    fi
    rm -f $TMP
}

docker images | grep $NGINX_IMAGE > /dev/null
if [[ $? != 0 ]]; then
    docker build -t $NGINX_IMAGE $PATH_TO_DOCKERFILE #create image
fi

ls $WEBSERV_DIR | grep webserv > /dev/null
if [[ $? != 0 ]]; then
    make -C $WEBSERV_DIR
fi

echo "" > $OUTPUT; #clear output
echo ""  > $ERROR_OUTPUT; #clear error

#change permission for test can't be applied immediatly because they cant be git push else
chmod 000 srcs/private
chmod 000 srcs/secret/mdp.html
chmod 000 srcs/spoiler/mdp.html


#PERSONAL ERROR PAGE TEST
#-d for run in background

#docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_ERROR_CONF:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME
#$WEBSERV $WS_ERROR_CONF

#WEBSERV_PID=$!
#sleep $SLEEP_TIMER
<< C2
launch_server $ERROR_CONF

#-L follow redirect -i http header in output -I only header
test $NAME_CONFIG "/"
test $NAME_CONFIG "/inexistant_file"
test $NAME_CONFIG "/secret/secret.html"


stop_server

#AUTOINDEX OFF TEST
launch_server $INDEX_OFF_CONF

test $NAME_CONFIG "/"
test $NAME_CONFIG "/html/3.html"

stop_server

#REDIRECT TEST
launch_server $REDIRECT_CONF

test $NAME_CONFIG "/" "-L"
test $NAME_CONFIG "/html/3.html" "-L"

stop_server

#MULTIPLE ERROR TEST
launch_server $SAME_ERROR_CONF

NAME_CONFIG=same_error.conf

test $NAME_CONFIG "/"
test $NAME_CONFIG "/ffgsdgsfgsg"
test $NAME_CONFIG "/secret/secret.html"
test $NAME_CONFIG "/html/3.html"

stop_server

#MULTIPLE LOCATION TEST
launch_server $MULTIPLE_LOCATION_CONF

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

<< HARD
test_put $NAME_CONFIG "/new.html" "-d '<p>impossible a tester c'est ok</p>'" #dont work
test_put $NAME_CONFIG "/private/impossible.html" "-d impossible2" #dont work
test_put $NAME_CONFIG "/secret/to_delete.html" "-d \"<p>secret</p>\"" #must do test
test_put $NAME_CONFIG "/no_path/new.html" "-d \"<p>error</p>\"" #dont work
test_put $NAME_CONFIG "/html/new.html" "-d \"<p>NO</p>\"" #work
test_put $NAME_CONFIG "/put_and_delete/page.html" "-d \"<p>OK alllez s'il te plait</p>\"" #work

cp -r srcs/dir_to_copy srcs/$NG_DELETE_DIR
cp -r srcs/dir_to_copy srcs/$WS_DELETE_DIR
test_delete $NAME_CONFIG "/cat_symbolic.html"
test_method $NAME_CONFIG GET "/dir_to_delete/cat.html" #test if symbolic link or file which are delete

test_delete $NAME_CONFIG "/cat.html"
test_delete $NAME_CONFIG "/1.html"
test_delete $NAME_CONFIG "/php/"
test_delete $NAME_CONFIG "/empty_dir/"
test_delete $NAME_CONFIG "/empty_dir/unexist" #unexist
test_delete $NAME_CONFIG "/"

test_delete $NAME_CONFIG "/private/"
#test_delete $NAME_CONFIG "/secret/to_delete.html" #dont do it

rm -rf  srcs/$NG_DELETE_DIR
rm -rf  srcs/$WS_DELETE_DIR

test_syntax syntax_ressources/wrong_uri
test_syntax syntax_ressources/line_feed
test_syntax syntax_ressources/line_feed2
test_syntax syntax_ressources/multiple_space
test_syntax syntax_ressources/space_and_tab
test_syntax syntax_ressources/tab
HARD
#test_post $NAME_CONFIG /php/1.php
#test_post $NAME_CONFIG /php/2.php
#test_post $NAME_CONFIG /php/exemple.php
#test_post $NAME_CONFIG /php/info.php
#test_method $NAME_CONFIG POST /php/php.php "-d arg1=O -d arg2=K -d arg3=!"
#test_method $NAME_CONFIG POST /php/php.php "-d arg1=ceci -d arg2=EST -d arg3=method -d arg4=POST"
#test_method $NAME_CONFIG GET /php/php.php "-G -d arg1=GET -d arg2=query -d arg3=STRING"

stop_server
C2

#PHP ERROR TEST
launch_server $PHP_ERROR_CONF

NAME_CONFIG=same_error_php.conf

test $NAME_CONFIG "/fdgfs" #404
test $NAME_CONFIG "/private/" #403
test $NAME_CONFIG "/secret/" #401
test $NAME_CONFIG "/html/cat.html" #405

stop_server


#MULTIPLE SERVER NAME TEST
#launch_multi_server $MULTIPLE_CONF

#test "default" "/"
#test "gif.conf" "/" "-H 'Host: gif'"
#test "jpeg.conf" "/" "-H 'Host: jpeg'"
#test "secret.conf" "/" "-H \"Host: secret\""

#stop_server

#permission reset

#permission
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html