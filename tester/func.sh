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

#test "name of config" "uri" ...(other options)
test ()
{
    test_method $1 GET "${@:2}"
    test_head "$@"
}

#test_method "name of config" "METHOD" "uri" ...(other options)
test_method ()
{
	declare HEADER_DIFF
	declare BODY_DIFF
	declare HEADER_DIFF_FILE=$TMP5
	declare BODY_DIFF_FILE=$TMP6

	curl -sSIX $2 "${@:4}" $NGINX_IP:$NGINX_PORT$3 > $TMP1
	curl -sSIX $2 "${@:4}" $WEBSERV_IP:$WEBSERV_PORT$3 > $TMP2
	curl -sSX $2 "${@:4}" $NGINX_IP:$NGINX_PORT$3 > $TMP3
	curl -sSX $2 "${@:4}" $WEBSERV_IP:$WEBSERV_PORT$3 > $TMP4
    diff -a $TMP1 $TMP2 > $TMP5
	HEADER_DIFF=$?
	diff $TMP3 $TMP4 > $TMP6
	BODY_DIFF=$?
	echo $BODY_DIFF
	echo $HEADER_DIFF
    if [[ $HEADER_DIFF ]] || [[ $BODY_DIFF ]]; then
        echo -e "----------------------" $1 " " $2 " " $3 " " "${@:4}" "------------------------\n" >> $OUTPUT
        echo -e $1 " " $2 " " $3 ": CHECK " $OUTPUT
    else
        echo -e $1 " " $2 " " $3 ": OK"
    fi
	if [[ HEADER_DIFF != 0 ]]; then
        cat $TMP5 >> $OUTPUT
	fi
	if [[ BODY_DIFF != 0 ]]; then
        cat $TMP6 >> $OUTPUT
    fi
    clear_x_tmpfile TMP 6
}

#test "name of config" "uri" ...(other options)
test_head ()
{
    declare TMP=$TMP1
    diff -a <(curl -sSIX HEAD "${@:3}" $NGINX_IP:$NGINX_PORT$2) <(curl -sSIX HEAD "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " HEAD " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " HEAD " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " HEAD " " $2 ": OK"
    fi
    clear_x_tmpfile TMP 1
}

#test "name_of_config" "path_to_test" ...(put param)
test_put()
{
    declare NGINX_TMP1=$TMP1
    declare NGINX_TMP2=$TMP2
    declare NGINX_TMP3=$TMP3
    declare WEBSERV_TMP1=$TMP4
    declare WEBSERV_TMP2=$TMP5
    declare WEBSERV_TMP3=$TMP6
    declare TMP=$TMP7

    curl -sSiIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP1
    curl -sSiIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP2
    curl -sSiIX GET $NGINX_IP:$NGINX_PORT$2 > $NGINX_TMP3
    rm -f ./srcs$2 > /dev/null #ignore if there is no permission
    curl -sSiIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP1
    curl -sSiIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP2
    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_TMP3
    rm -f ./srcs$2 >/dev/null #ignore if there is no permission

    diff -a $NGINX_TMP1 $WEBSERV_TMP1 > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " PUT1 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " PUT1 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " PUT1 " " $2 ": OK"
    fi
    diff -a $NGINX_TMP2 $WEBSERV_TMP2 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " " PUT2 " " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " PUT2 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " PUT2 " " $2 ": OK"
    fi
    diff -a $NGINX_TMP3 $WEBSERV_TMP3 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER PUT " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo $1 " " GET AFTER PUT " " $2 ": CHECK " $OUTPUT
    else
        echo $1 " " GET AFTER PUT " " $2 ": OK"
    fi

    clear_x_tmpfile TMP 7
}

#test "name_of_config" "path_to_test"
test_delete()
{
    declare NG_PATH=$NG_DELETE_DIR$2
    declare WS_PATH=$WS_DELETE_DIR$2

    declare NGINX_TMP1=$TMP1
    declare NGINX_TMP2=$TMP2
    declare NGINX_TMP3=$TMP3
    declare NGINX_TMP4=$TMP4
    declare WEBSERV_TMP1=$TMP5
    declare WEBSERV_TMP2=$TMP6
    declare WEBSERV_TMP3=$TMP7
    declare WEBSERV_TMP4=$TMP8
    declare TMP=$TMP9

    curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP1
    curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP2
	curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP3
    curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP4

    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP1
    curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH >$WEBSERV_TMP2
	curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP3
    curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP4


    diff -a $NGINX_TMP1 $WEBSERV_TMP1 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET BEFORE DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " GET BEFORE DELETE " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " GET BEFORE DELETE " " $2 ": OK"
    fi
    diff -a $NGINX_TMP2 $WEBSERV_TMP2 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE 1" $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " DELETE1 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " DELETE1 " " $2 ": OK"
    fi
    diff -a $NGINX_TMP3 $WEBSERV_TMP3 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " DELETE2 " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " DELETE2 " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " DELETE2 " " $2 ": OK"
    fi
    diff -a $NGINX_TMP4 $WEBSERV_TMP4 > $TMP
    if [[ $? != 0 ]]; then 
        echo -e "----------------------" $1 " GET AFTER DELETE " $2 "------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " GET AFTER DELETE " " $2 ": CHECK " $OUTPUT
    else
        echo -e $1 " " GET AFTER DELETE " " $2 ": OK"
    fi

	clear_x_tmpfile TMP 9
}

#test_syntax file 
test_syntax()
{
    declare TMP=$TMP1

    diff -a <(python send_request.py $1 $NGINX_IP $NGINX_PORT) <(python send_request.py $1 $WEBSERV_IP $WEBSERV_PORT) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " SYNTAX_TEST  " " "----------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " SYNTAX_TEST " : CHECK " $OUTPUT
    else
        echo -e $1 " " SYNTAX_TEST  " : OK"
    fi
    clear_x_tmpfile TMP 1
}

generate_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
	true #force first passage
	while [[ $? == 0 ]]; do
		export $1$i=$3$(cat /dev/urandom | env LC_CTYPE=C tr -cd 'a-f0-9' | head -c 32)
		TMP=$1$i
		ls ${!TMP} > /dev/null 2> /dev/null
	done

	touch ${!TMP}
	#ls ${!TMP} #> /dev/null 2> /dev/null
	if [[ $? != 0 ]]; then
		delete_x_tmpfile $(expr $i - 1)
		echo "file creation failed"
		exit 1;
	fi
	done
}

generate_x_tmpdir()
{
	for ((i = 1; i < $2 + 1; i++)) do
	true #force first passage
	while [[ $? == 0 ]]; do
		export $1$i=$(cat /dev/urandom | env LC_CTYPE=C tr -cd 'a-f0-9' | head -c 32)
		TMP=$1$i
		ls ${!TMP} > /dev/null 2> /dev/null
	done

	mkdir ${!TMP}
	#ls ${!TMP} #> /dev/null 2> /dev/null
	if [[ $? != 0 ]]; then
		delete_x_tmpfile $(expr $i - 1)
		echo "file creation failed"
		exit 1;
	fi
	done
}

delete_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
		TMP=$1$i
		rm -f $3${!TMP}
	done
}

delete_x_tmpdir()
{
	for ((i = 1; i < $2 + 1; i++)) do
		TMP=$1$i
		rm -rf ${!TMP}
	done
}

clear_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
		TMP=$1$i
		echo "" > $3${!TMP}
	done
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