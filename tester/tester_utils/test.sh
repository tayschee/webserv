#test "name of config" "uri" ...(other options)
test ()
{
    test_method "$1" "GET" "${@:2}"
    test_head "$@"
}

#test_method "name of config" "METHOD" "uri" ...(other options)
test_method ()
{
	get_response "1" "$NGINX_IP" "$NGINX_PORT" "${@:2}"
	get_response "3" "$WEBSERV_IP" "$WEBSERV_PORT" "${@:2}"
 
	print_diff 1 3 5 "${@}"
    clear_x_tmpfile TMP 6
}

#test "name of config" "uri" ...(other options)
test_head ()
{
    declare TMP=$TMP1
    diff -y --suppress-common-lines -a <(curl  -H "Host: $SERVER_NAME" -sSIX HEAD "${@:3}" $NGINX_IP:$NGINX_PORT$2) <(curl  -H "Host: $SERVER_NAME" -sSIX HEAD "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2) > $TMP
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
	declare GET_OPTIONS=$(expr "$2" + 1)
	declare PUT_OPTIONS=$(expr "$2" - 2)

	#TEST FIRST PUT
	get_response "1" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "3" "$NGINX_IP" "$NGINX_PORT" "PUT" "${@:3:$PUT_OPTIONS}"

	#TEST SECOND PUT
	get_response "5" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "7" "$NGINX_IP" "$NGINX_PORT" "PUT"  "${@:3:$PUT_OPTIONS}"
	get_response "9" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"

    rm -f ./srcs$PUT_OPTIONS > /dev/null #ignore if there is no permission


	#TEST FIRST PUT
	get_response "11" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "13" "$WEBSERV_IP" "$WEBSERV_PORT" "PUT"  "${@:3:$PUT_OPTIONS}"

	#TEST SECOND PUT
	get_response "15" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "17" "$WEBSERV_IP" "$WEBSERV_PORT" "PUT"  "${@:3:$PUT_OPTIONS}"
	get_response "19" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"

	rm -f ./srcs$PUT_OPTIONS >/dev/null #ignore if there is no permission


	print_diff 1 11 21 $1 GET_BEFORE_PUT_1 "$3" "${@:$GET_OPTIONS}"
	print_diff 3 13 21 $1 PUT_1 "${@:3:$PUT_OPTIONS}"
	print_diff 5 15 21 $1 GET_AFTER_PUT_1 "$3" "${@:$GET_OPTIONS}"
	print_diff 7 17 21 $1 PUT_2 "${@:3:$PUT_OPTIONS}"
	print_diff 9 19 21 $1 GET_AFTER_PUT_2 "$3" "${@:$GET_OPTIONS}"

	clear_x_tmpfile TMP 22
}

#test "name_of_config" "path_to_test"
test_delete()
{
	declare GET_OPTIONS=$(expr "$2" + 1)
	declare DELETE_OPTIONS=$(expr "$2" - 5)

	cp -r "$3" "$5"
	chmod "$4" "$5"

	#TEST FIRST DELETE
	get_response "1" "$NGINX_IP" "$NGINX_PORT" "GET" "$6" "${@:$GET_OPTIONS}"
	get_response "3" "$NGINX_IP" "$NGINX_PORT" "DELETE" "${@:6:$DELETE_OPTIONS}"

	#TEST SECOND DELETE
	get_response "5" "$NGINX_IP" "$NGINX_PORT" "GET" "$6" "${@:$GET_OPTIONS}"
	get_response "7" "$NGINX_IP" "$NGINX_PORT" "DELETE" "${@:6:$DELETE_OPTIONS}"
	get_response "9" "$NGINX_IP" "$NGINX_PORT" "GET" "$6" "${@:$GET_OPTIONS}"

	chmod 777 "$5"
    rm -rf $5 > /dev/null #ignore if there is no permission

	cp -r "$3" "$5"
	chmod "$4" "$5"

	#TEST FIRST DELETE
	get_response "11" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$6" "${@:$GET_OPTIONS}"
	get_response "13" "$WEBSERV_IP" "$WEBSERV_PORT" "DELETE" "${@:6:$DELETE_OPTIONS}"

	#TEST SECOND DELETE
	get_response "15" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$6" "${@:$GET_OPTIONS}"
	get_response "17" "$WEBSERV_IP" "$WEBSERV_PORT" "DELETE" "${@:6:$DELETE_OPTIONS}"
	get_response "19" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$6" "${@:$GET_OPTIONS}"

	chmod 777 "$5"
    rm -rf $5 > /dev/null #ignore if there is no permission


	print_diff 1 11 21 $1 GET_BEFORE_DELETE_1 "$6" "${@:$GET_OPTIONS}"
	print_diff 3 13 21 $1 DELETE_1  "${@:6:$DELETE_OPTIONS}"
	print_diff 5 15 21 $1 GET_AFTER_DELETE_1 "$6" "${@:$GET_OPTIONS}"
	print_diff 7 17 21 $1 DELETE_2  "${@:6:$DELETE_OPTIONS}"
	print_diff 9 19 21 $1 GET_AFTER_DELETE_2 "$6" "${@:$GET_OPTIONS}"

    clear_x_tmpfile TMP 22
}

#test_syntax file 
test_syntax()
{
    declare NG_REP=$TMP1
	declare NG_HEAD=$TMP2
	declare NG_BODY=$TMP3

	declare WS_REP=$TMP4
	declare WS_HEAD=$TMP5
	declare WS_BODY=$TMP6


	python $SEND_REQUEST_PY $1 $NGINX_IP $NGINX_PORT > $NG_REP
	python $SPLIT_RESPONSE_PY $NG_REP $NG_HEAD $NG_BODY


	python $SEND_REQUEST_PY $1 $WEBSERV_IP $WEBSERV_PORT > $WS_REP
	python $SPLIT_RESPONSE_PY $WS_REP  $WS_BODY$WS_HEAD

	print_diff 2 5 7 "$@"

	sleep 20

	clear_x_tmpfile TMP 8
}