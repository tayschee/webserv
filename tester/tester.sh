source tester_utils/include.sh
source tester_utils/launch_stop.sh
source tester_utils/random_file.sh
source tester_utils/test.sh
source tester_utils/utils.sh

echo "" > $OUTPUT; #clear output
#echo ""  > $ERROR_OUTPUT; #clear error


setup_server
trap stop_prog SIGINT
#change permission for test can't be applied immediatly because they cant be git push else
chmod 000 srcs/private/
chmod 000 srcs/secret/mdp.html
chmod 000 srcs/spoiler/mdp.html

generate_x_tmpdir DIR_TMP 1 #generate a directory it name is in $DIR_TMP1
generate_x_tmpfile TMP 22 $DIR_TMP1/ #generate 9 file with random name in $DIR_TMP1/

#launch server_nginx with config in nginx_config and webserv with webserv_config
launch_multi_server

SERVER_NAME=error

test $SERVER_NAME "/"
test $SERVER_NAME "/inexistant_file"
test $SERVER_NAME "/secret/"
test $SERVER_NAME "/secret/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $SERVER_NAME "/secret/" -H "Authorization: Basic gegfgrsrwwrrw"
test $SERVER_NAME "/secret/" -H "Authorization: NOT_BASIC YWRtaW46YWRtaW4="
test $SERVER_NAME "/secret/" -H "Authorization: HTDHRTHDGF GTRGHE YTREYE YWRtaW46YWRtaW4="
test $SERVER_NAME "/secret/" -H "Authorization: Basic YWRtaW46YWRtaW4= YTREYE"
test $SERVER_NAME "/bad_secret/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $SERVER_NAME "/bad_secret2/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $SERVER_NAME "/secret/secret.html"
test $SERVER_NAME "/private/private.html"
test $SERVER_NAME "/private/"

SERVER_NAME=index_off

test $SERVER_NAME "/"
test $SERVER_NAME "/html/3.html"


SERVER_NAME=redirect

test $SERVER_NAME "/"
test $SERVER_NAME "/html/"
test $SERVER_NAME "/gif/"
test $SERVER_NAME "/jpeg/"
test $SERVER_NAME "/error/"
test $SERVER_NAME "/dest/"
test $SERVER_NAME "/" "-L"
test $SERVER_NAME "/html/3.html" "-L"

SERVER_NAME=same_error

test $SERVER_NAME "/"
test $SERVER_NAME "/ffgsdgsfgsg"
test $SERVER_NAME "/secret/secret.html"
test $SERVER_NAME "/html/3.html"

SERVER_NAME=multiple_location
 
test $SERVER_NAME "/"
test $SERVER_NAME "/gif/"
test $SERVER_NAME "/html/"
test $SERVER_NAME "/jpeg/"
test $SERVER_NAME "/png/"
test $SERVER_NAME "/private/"
test $SERVER_NAME "/secret/"
test $SERVER_NAME "/spoiler/"
test $SERVER_NAME "/error/"
test $SERVER_NAME "/php/"
test $SERVER_NAME "/html/3.html"
test $SERVER_NAME "/unexist.html"

PUT TEST
test_put $SERVER_NAME 5 "/new.html" -d "<p>little</p>"
test_put $SERVER_NAME 5 "/private/impossible.html" -d "<p>une phrase un peu plus longue</p>" #dont work
test_put $SERVER_NAME 7 "/secret/new.html" -d "<p>secret</p>" -H "Authorization: Basic YWRtaW46YWRtaW4=" -H "Authorization: Basic YWRtaW46YWRtaW4="
test_put $SERVER_NAME 5 "/spoiler/new.html" -d "<p>spoil</p>" #must do test
test_put $SERVER_NAME 7 "/spoiler/new.html" -d "<p>spoil</p>" -H "Authorization: Basic YWRtaW46YWRtaW4=" -H "Authorization: Basic YWRtaW46YWRtaW4="
test_put $SERVER_NAME 5 "/no_path/new.html" -d "<p>error</p>" #dont work
test_put $SERVER_NAME 5 "/html/new.html" -d "<p>YES</p>" #work
test_put $SERVER_NAME 5 "/put_and_delete/page.html" -d "<p>QUELQUE CHOSE D'UN PEU PLUS LONG QUE LE RESTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!</p>" #work

# #DELETE TEST
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/html/1.html"
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html"
test_delete $SERVER_NAME 8 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html" -H "Authorization: Basic YWRtaW46YWRtaW4=" -H "Connection : close" -H "Authorization: Basic YWRtaW46YWRtaW4="
test_delete $SERVER_NAME 8 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html" -H "Authorization: Basic gfgfsgs" -H "Connection : close" -H "Authorization: Basic YWRtaW46YWRtaW4="
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/gif/"
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/"

test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/php/1.php"
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/gif/"
test_delete $SERVER_NAME 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/"

test_syntax syntax_ressources/wrong_uri
test_syntax syntax_ressources/line_feed
test_syntax syntax_ressources/line_feed2
test_syntax syntax_ressources/multiple_space
test_syntax syntax_ressources/space_and_tab
test_syntax syntax_ressources/tab

test_method $SERVER_NAME POST /php/1.php
test_method $SERVER_NAME POST /php/2.php
test_method $SERVER_NAME POST /php/exemple.php
test_method $SERVER_NAME /php/info.php
test_method $SERVER_NAME POST /php/php.php -d arg1=O -d arg2=K -d arg3=!
test_method $SERVER_NAME POST /php/php.php -d arg1=ceci -d arg2=EST -d arg3=method -d arg4=POST
test_method $SERVER_NAME GET /php/php.php -G -d arg1=GET -d arg2=query -d arg3=STRING

SERVER_NAME=same_error_php

test $SERVER_NAME "/fdgfs" #404
test $SERVER_NAME "/private/" #403
test $SERVER_NAME "/secret/" #401
test $SERVER_NAME "/html/cat.html" #405

stop_server
delete_x_tmpdir DIR_TMP 1

#permission reset

#permission
chmod 755 srcs/private/ #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html