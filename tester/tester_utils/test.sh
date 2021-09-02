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
	declare DELETE_OPTIONS=$(expr "$2" - 2)

	echo GET OPTIONS : $GET_OPTIONS
	echo VERIF "${@:3:$DELETE_OPTIONS}"
	echo VERIF2 "${@:$GET_OPTIONS}"
	#TEST FIRST DELETE
	get_response "1" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "3" "$NGINX_IP" "$NGINX_PORT" "DELETE" "${@:3:$DELETE_OPTIONS}"

	#TEST SECOND DELETE
	get_response "5" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "7" "$NGINX_IP" "$NGINX_PORT" "DELETE"  "${@:3:$DELETE_OPTIONS}"
	get_response "9" "$NGINX_IP" "$NGINX_PORT" "GET" "$3" "${@:$GET_OPTIONS}"

    rm -f ./srcs$DELETE_OPTIONS > /dev/null #ignore if there is no permission


	#TEST FIRST DELETE
	get_response "11" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "13" "$WEBSERV_IP" "$WEBSERV_PORT" "DELETE"  "${@:3:$DELETE_OPTIONS}"

	#TEST SECOND DELETE
	get_response "15" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"
	get_response "17" "$WEBSERV_IP" "$WEBSERV_PORT" "DELETE"  "${@:3:$DELETE_OPTIONS}"
	get_response "19" "$WEBSERV_IP" "$WEBSERV_PORT" "GET" "$3" "${@:$GET_OPTIONS}"

	rm -f ./srcs$DELETE_OPTIONS >/dev/null #ignore if there is no permission


	print_diff 1 11 21 $1 GET_BEFORE_DELETE_1 "$3" "${@:$GET_OPTIONS}"
	print_diff 3 13 21 $1 DELETE_1 "${@:3:$DELETE_OPTIONS}"
	print_diff 5 15 21 $1 GET_AFTER_DELETE_1 "$3" "${@:$GET_OPTIONS}"
	print_diff 7 17 21 $1 DELETE_2 "${@:3:$DELETE_OPTIONS}"
	print_diff 9 19 21 $1 GET_AFTER_DELETE_2 "$3" "${@:$GET_OPTIONS}"

    clear_x_tmpfile TMP 22
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