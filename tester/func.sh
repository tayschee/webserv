source include.sh

#test "name_of_config" "METHOD" "path_to_test" ...()
#launch_server "config_file for nginx"

setup_server()
{
	docker images | grep $NGINX_IMAGE > /dev/null
	if [[ $? != 0 ]]; then
    	docker build -t $NGINX_IMAGE $PATH_TO_DOCKERFILE #create image
		if [[ $? != 0 ]]; then
			echo "Creation of Docker image failed"
			exit 1
		fi
	fi

	ls $WEBSERV_DIR | grep webserv > /dev/null
	if [[ $? != 0 ]]; then
    	make -C $WEBSERV_DIR
		if [[ $? != 0 ]]; then
			echo "Executable " $WEBSERV " could not be created."
			exit 1
		fi
	fi
}

launch_server()
{
    CONTAINER_NAME=$(docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME)

    if [[ $! != 0 ]]; then
        echo -e "\nNginx run $1\n"
    else
        echo -e "Nginx failed\n"
        exit 1
    fi
    #$WEBSERV $WS_DIRECTORY_CONF/$1 &

    #WEBSERV_PID=$!
    NAME_CONFIG=$1
    sleep $SLEEP_TIMER
}

#launch_multi_server "config_dir for nginx"
launch_multi_server()
{
    CONTAINER_NAME=$(docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH_DIR -p 80:80 --name $CONTAINER_NAME $IMAGE_NAME)
    if [[ $! != 0 ]]; then
        echo -e "\nNginx run\n"
    else
        echo -e "Nginx failed\n"
        exit 1
    fi
    #$WEBSERV $WS_DIRECTORY_CONF/$1 &

    #WEBSERV_PID=$!
    sleep $SLEEP_TIMER
}

#use to stop nginx and webserv
stop_server()
{
    docker stop $CONTAINER_NAME
    #kill -2 $WEBSERV_PID #sigint webserv
}

#test "name of config" "uri" ...(other options)
test ()
{
    test_method $1 GET "${@:2}"
    test_head "$@"
}

#test_method "name of config" "METHOD" "uri" ...(other options)
test_method ()
{
    diff <(curl -sSiX $2 "${@:4}" $NGINX_IP:$NGINX_PORT$3) <(curl -sSiX $2 "${@:4}" $WEBSERV_IP:$WEBSERV_PORT$3) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " $2 " " $3 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " $2 " " $3 ": CHECK " $OUTPUT
    else
        echo -e $1 " " $2 " " $3 ": OK"
    fi
    rm -f $TMP
}

#test "name of config" "uri" ...(other options)
test_head ()
{
    diff <(curl -sSiIX HEAD "${@:3}" $NGINX_IP:$NGINX_PORT$2) <(curl -sSiIX HEAD "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " HEAD " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " HEAD " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " HEAD " " $2 ": OK"
    fi
    rm -f $TMP
}

#test "name_of_config" "path_to_test" ...(put param)
test_put()
{
    curl -sSiIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"1"
    curl -sSiIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"2"
    curl -sSiIX GET $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP"3"
    rm -f ./srcs$2
    curl -sSiIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"1"
    curl -sSiIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"2"
    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP"3"

    diff $WEBSERV_TMP"1" $NGINX_TMP"1" > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " PUT1 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " PUT1 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " PUT1 " " $2 ": OK"
    fi
    diff $WEBSERV_TMP"2" $NGINX_TMP"2" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " " PUT2 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " PUT2 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " PUT2 " " $2 ": OK"
    fi
    diff $WEBSERV_TMP"3" $NGINX_TMP"3" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER PUT " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo $1 " " GET AFTER PUT " " $2 ": CHECK " $OUTPUT
    else
        echo $1 " " GET AFTER PUT " " $2 ": OK"
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

#test "name_of_config" "path_to_test"
test_delete()
{
    NG_PATH=$NG_DELETE_DIR$2
    WS_PATH=$WS_DELETE_DIR$2

	for i in 9; do
		false #force first passage
		while ($? != 0)
			TMP$i=$(cat /dev/urandom | env LC_CTYPE=C tr -cd 'a-f0-9' | head -c 32)
			ls TMP$i > /dev/null 2> /dev/null

		touch TMP$i
		if ($? != 0); then
			for j in i; do
				rm -f $(TMP)$(j)
			done
			exit 1;
		fi
	done

    curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $TMP"1"
    curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $TMP"2"
	curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $TMP"3"
    curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $TMP"4"

    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $TMP"5"
    curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $TMP"6"
	curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $TMP"7"
    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $TMP"8"


    diff $TMP"1" $TMP"5" > $TMP"9"
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET BEFORE DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP"9" >> $OUTPUT
        echo -e $1 " " GET BEFORE DELETE " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " GET BEFORE DELETE " " $2 ": OK"
    fi
    diff $TMP"2" $TMP"6" > $TMP"9"
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE 1" $2 "------------------------\n" >> $OUTPUT
        cat $TMP"9" >> $OUTPUT
        echo -e $1 " " DELETE1 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " DELETE1 " " $2 ": OK"
    fi
    diff $TMP"3" $TMP"7" > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE2 " $2 "------------------------\n" >> $OUTPUT
        cat $TMP"9" >> $OUTPUT
        echo -e $1 " " DELETE2 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " DELETE2 " " $2 ": OK"
    fi
    diff $TMP"4" $TMP"8" > $TMP"9"
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP"9" >> $OUTPUT
        echo -e $1 " " GET AFTER DELETE " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " GET AFTER DELETE " " $2 ": OK"
    fi

    rm -f $TMP"1"
    rm -f $TMP"2" 
    rm -f $TMP"3" 
    rm -f $TMP"4" 
    rm -f $TMP"5"
    rm -f $TMP"6" 
    rm -f $TMP"7" 
    rm -f $TMP"8"

    rm -f $TMP"9"

}

#test_syntax file 
test_syntax()
{
    diff <(python send_request.py $1 $NGINX_IP $NGINX_PORT) <(python send_request.py $1 $WEBSERV_IP $WEBSERV_PORT) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " SYNTAX_TEST  " " "----------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " SYNTAX_TEST " : CHECK " $OUTPUT
    else
        echo -e $1 " " SYNTAX_TEST  " : OK"
    fi
    rm -f $TMP
}