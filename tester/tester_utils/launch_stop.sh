#source include.sh

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
    CONTAINER_NAME=$(docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH -p 80:80 $IMAGE_NAME)

    if [[ $! != 0 ]]; then
        echo -e "\nNginx run $1\n"
    else
        echo -e "Nginx failed\n"
        exit 1
    fi
    $WEBSERV $WS_DIRECTORY_CONF/$1 &

    WEBSERV_PID=$!
    NAME_CONFIG=$1
    sleep $SLEEP_TIMER
}

#launch_multi_server "config_dir for nginx"
launch_multi_server()
{
    CONTAINER_NAME=$(docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/$1:$VM_CONFIG_PATH_DIR -p 80:80 $IMAGE_NAME)
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
    kill -2 $WEBSERV_PID #sigint webserv
}

stop_prog()
{
	if (( ! -z $CONTAINER_NAME && docker ps  | grep $CONTAINER_NAME > dev/null )); then
		docker stop $CONTAINER_NAME
	fi
	#if [[ (! -z $WEBSERV_PID) && ps | grep $WEBSERV_PID > dev/null ]]
#	{
#		kill -2 $WEBSERV_PID
#	}

	delete_x_tmpdir DIR_TMP 1
	rm -rf  srcs/$NG_DELETE_DIR
	rm -rf  srcs/$WS_DELETE_DIR
	exit 1
}