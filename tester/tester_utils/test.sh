#test "name of config" "uri" ...(other options)
test ()
{
    test_method "$1" "GET" "${@:2}"
    #test_head "$@"
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
    diff -y --suppress-common-lines -a <(curl -sSIX HEAD "${@:3}" $NGINX_IP:$NGINX_PORT$2) <(curl -sSIX HEAD "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2) > $TMP
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
	get_response "1" "$NGINX_IP" "$NGINX_PORT" "GET" #"${@:2}"

	get_response "3" "$NGINX_IP" "$NGINX_PORT" "PUT" "${@:2}"
    #curl -sSIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_HEAD1
	#curl -sSX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_BODY1

	#get_response "5" "$NGINX_IP" "$NGINX_PORT" GET ${@:2}	
    #curl -sSIX GET $NGINX_IP:$NGINX_PORT$2 > $NGINX_HEAD3
	#curl -sSIX GET $NGINX_IP:$NGINX_PORT$2 > $NGINX_BODY3

	#get_response "7" "$NGINX_IP" "$NGINX_PORT" PUT ${@:2}
    #curl -sSIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_HEAD2
	#curl -sSX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_BODY2

	#get_response "9" "$NGINX_IP" "$NGINX_PORT" GET ${@:2}
    #curl -sSIX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_HEAD2
	#curl -sSX PUT "${@:3}" $NGINX_IP:$NGINX_PORT$2 > $NGINX_BODY2

    rm -f ./srcs$2 > /dev/null #ignore if there is no permission

	get_response "11" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" #"${@:2}"

    get_response "13" "$WEBSERV_IP" "$WEBSERV_PORT" "PUT" "${@:2}"
    #curl -sSIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_HEAD1
	#curl -sSX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_BODY1

	#get_response "15" "$WEBSERV_IP" "$WEBSERV_PORT" GET ${@:2}	
    #curl -sSIX GET $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_HEAD3
	#curl -sSIX GET $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_BODY3

	#get_response "17" "$WEBSERV_IP" "$WEBSERV_PORT" PUT ${@:2}
    #curl -sSIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_HEAD2
	#curl -sSX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_BODY2

	#get_response "19" "$WEBSERV_IP" "$WEBSERV_PORT" GET ${@:2}
    #curl -sSIX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_HEAD2
	#curl -sSX PUT "${@:3}" $WEBSERV_IP:$WEBSERV_PORT$2 > $WEBSERV_BODY2
    rm -f ./srcs$2 >/dev/null #ignore if there is no permission

	print_diff 1 11 21 $1 "GET_BEFORE_PUT_1" "${@:2}"
	print_diff 3 13 21 $1 PUT_1 "${@:3}"
	#print_diff 5 15 21 $1 GET_AFTER_PUT_1 "${@:3}"
	#print_diff 7 17 21 $1 PUT_2 "${@:3}"
	#print_diff 9 19 21 $1 GET_AFTER_PUT_2 "${@:3}"

    clear_x_tmpfile TMP 22
}

#test "name_of_config" "path_to_test"
test_delete()
{

	get_response "1" "$NGINX_IP" "$NGINX_PORT" "GET" "/$NG_PATH" ${@:3} 
    #curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP1

	get_response "3" "$NGINX_IP" "$NGINX_PORT" "DELETE" "/$NG_PATH" ${@:3} 
	#curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP2

	get_response "5" "$NGINX_IP" "$NGINX_PORT" "GET" "/$NG_PATH" ${@:3}
	#curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP3

	get_response "7" "$NGINX_IP" "$NGINX_PORT" "DELETE" "/$NG_PATH" ${@:3}
    #curl -sSiIX GET $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP4

	get_response "9" "$NGINX_IP" "$NGINX_PORT" "GET" "/$NG_PATH" ${@:3}
	#curl -sSiIX DELETE $NGINX_IP:$NGINX_PORT/$NG_PATH > $NGINX_TMP3


	get_response "11" "$NGINX_IP" "$NGINX_PORT" "GET" "/$WS_PATH" ${@:3}
    #curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP1
	get_response "13" "$NGINX_IP" "$NGINX_PORT" "DELETE" "/$WS_PATH" ${@:3}
    #curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH >$WEBSERV_TMP2
	get_response "15" "$NGINX_IP" "$NGINX_PORT" "GET" "/$WS_PATH" ${@:3}
	#curl -sSiIX DELETE $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP3
	get_response "17" "$NGINX_IP" "$NGINX_PORT" "DELETE" "/$WS_PATH" ${@:3}
    #curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP4
	get_response "19" "$NGINX_IP" "$NGINX_PORT" "GET" "/$WS_PATH" ${@:3}
    #curl -sSiIX GET $WEBSERV_IP:$WEBSERV_PORT/$WS_PATH > $WEBSERV_TMP4


	print_diff 1 11 21 $1 GET_BEFORE_DELETE_1 "${@:3}"
	print_diff 3 13 21 $1 PUT_1 "${@:3}"
	print_diff 5 15 21 $1 GET_AFTER_PUT_1 "${@:3}"
	print_diff 7 17 21 $1 PUT_2 "${@:3}"
	print_diff 9 19 21 $1 GET_AFTER_PUT_2 "${@:3}"

	clear_x_tmpfile TMP 9
}

#test_syntax file 
test_syntax()
{
    declare TMP=$TMP1

    diff -y --suppress-common-lines -a <(python $SEND_REQUEST_PY $1 $NGINX_IP $NGINX_PORT) <(python $SEND_REQUEST_PY $1 $WEBSERV_IP $WEBSERV_PORT) > $TMP
    if [[ $? != 0 ]]; then
        echo -e "----------------------" $1 " " SYNTAX_TEST  " " "----------------------------\n" >> $OUTPUT
        cat $TMP >> $OUTPUT
        echo -e $1 " " SYNTAX_TEST " : CHECK " $OUTPUT
    else
        echo -e $1 " " SYNTAX_TEST  " : OK"
    fi
    clear_x_tmpfile TMP 1
}