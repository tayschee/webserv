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

	ls $WEBSERV > /dev/null
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

	pgrep -x $WEBSERV_PID

	#check if webserv quit
	if [[ $! != 0 ]]; then
        echo -e "\nWebserv run $1\n"
    else
        echo -e "Webserv failed\n"
		docker stop $CONTAINER_NAME
        exit 1
    fi

}

#launch_multi_server "config_dir for nginx"
launch_multi_server()
{
    CONTAINER_NAME=$(docker run --rm -ti -d -v $SRCS_PATH:$VM_SRCS_PATH -v $NG_DIRECTORY_CONF/:$VM_CONFIG_PATH_DIR -p 80:80 $IMAGE_NAME)
    if [[ $! != 0 ]]; then
        echo -e "\nNginx run\n"
    else
        echo -e "\nNginx failed\n"
        exit 1
    fi

	echo webserv = $WEBSERV WS_DIRECTORY_CONF = $WS_DIRECTORY_CONF

    $WEBSERV $WS_DIRECTORY_CONF &
	WEBSERV_PID=$!
    sleep $SLEEP_TIMER

	pgrep -x $WEBSERV_PID

	if [[ $! != 0 ]]; then
        echo -e "\nWebserv run $1\n"
    else
        echo -e "\nWebserv failed\n"
		docker stop $CONTAINER_NAME
        exit 1
    fi
}

#use to stop nginx and webserv
stop_server()
{
    docker stop $CONTAINER_NAME
    kill -2 $WEBSERV_PID #sigint webserv
}

stop_prog()
{
	docker stop $CONTAINER_NAME > /dev/null 2> /dev/null
	kill -2 $WEBSERV_PID > /dev/null 2> /dev/null

	delete_x_tmpdir DIR_TMP 1
	rm -rf  srcs/$NG_DELETE_DIR
	rm -rf  srcs/$WS_DELETE_DIR

	chmod 755 srcs/private #because it s a directory
	chmod 644 srcs/secret/mdp.html
	chmod 644 srcs/spoiler/mdp.html
	exit 1
}