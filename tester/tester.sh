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
chmod 000 srcs/private
chmod 000 srcs/secret/mdp.html
chmod 000 srcs/spoiler/mdp.html

generate_x_tmpdir DIR_TMP 1 #generate a directory it name is in $DIR_TMP1
generate_x_tmpfile TMP 22 $DIR_TMP1/ #generate 9 file with random name in $DIR_TMP1/

<<TEST
# <<C

launch_server $ERROR_CONF #lancer la config sur webserv et nginx le parametre est le nom du dosier dans webserv et le nom du fichier de la config de nginx

#-L follow redirect -i http header in output -I only header
test $NAME_CONFIG "/"
test $NAME_CONFIG "/inexistant_file"
test $NAME_CONFIG "/secret/"
test $NAME_CONFIG "/secret/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $NAME_CONFIG "/secret/" -H "Authorization: Basic gegfgrsrwwrrw"
test $NAME_CONFIG "/secret/" -H "Authorization: NOT_BASIC YWRtaW46YWRtaW4="
test $NAME_CONFIG "/secret/" -H "Authorization: HTDHRTHDGF GTRGHE YTREYE YWRtaW46YWRtaW4="
test $NAME_CONFIG "/secret/" -H "Authorization: Basic YWRtaW46YWRtaW4= YTREYE"
test $NAME_CONFIG "/bad_secret/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $NAME_CONFIG "/bad_secret2/" -H "Authorization: Basic YWRtaW46YWRtaW4="
test $NAME_CONFIG "/secret/secret.html"
test $NAME_CONFIG "/private/private.html"
test $NAME_CONFIG "/private/"

stop_server

C

# <<C

#AUTOINDEX OFF TEST
launch_server $INDEX_OFF_CONF

test $NAME_CONFIG "/"
test $NAME_CONFIG "/html/3.html"

stop_server

#REDIRECT TEST
launch_server $REDIRECT_CONF

test $NAME_CONFIG "/"
test $NAME_CONFIG "/html/"
test $NAME_CONFIG "/gif/"
test $NAME_CONFIG "/jpeg/"
test $NAME_CONFIG "/error/"
test $NAME_CONFIG "/dest/"
test $NAME_CONFIG "/" "-L"
test $NAME_CONFIG "/html/3.html" "-L"

stop_server

# C

# <<C

#MULTIPLE ERROR TEST
launch_server $SAME_ERROR_CONF

NAME_CONFIG=same_error.conf

test $NAME_CONFIG "/"
test $NAME_CONFIG "/ffgsdgsfgsg"
test $NAME_CONFIG "/secret/secret.html"
test $NAME_CONFIG "/html/3.html"


stop_server

# C

#MULTIPLE LOCATION TEST
launch_server $MULTIPLE_LOCATION_CONF

NAME_CONFIG=multiple_location.conf

<< C
test $NAME_CONFIG "/"
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

test_put $NAME_CONFIG 5 "/new.html" -d "<p>little</p>" -l
test_put $NAME_CONFIG 5 "/private/impossible.html" -d "<p>une phrase un peu plus longue</p>" #dont work
test_put $NAME_CONFIG 5 "/secret/to_delete.html" -d "<p>secret</p>" #must do test
test_put $NAME_CONFIG 5 "/no_path/new.html" -d "<p>error</p>" #dont work
test_put $NAME_CONFIG 5 "/html/new.html" -d "<p>YES</p>" #work
test_put $NAME_CONFIG 5 "/put_and_delete/page.html" -d "<p>QUELQUE CHOSE D'UN PEU PLUS LONG QUE LE RESTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!</p>" #work

stop_server

C

#DELETE TEST

#test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/html/1.html"
#test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html"
#test_delete $NAME_CONFIG 8 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html" -H "Authorization: Basic YWRtaW46YWRtaW4=" -H "Connection : close" -H "Authorization: Basic YWRtaW46YWRtaW4="
#test_delete $NAME_CONFIG 8 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/secret/secret.html" -H "Authorization: Basic gfgfsgs" -H "Connection : close" -H "Authorization: Basic YWRtaW46YWRtaW4="
# test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/gif/"
# test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 755 "srcs/dir_to_delete" "/dir_to_delete/"

# test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/php/1.php"
# test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/gif/"
# test_delete $NAME_CONFIG 6 "srcs/dir_to_copy" 000 "srcs/dir_to_delete" "/dir_to_delete/"

#test_syntax syntax_ressources/wrong_uri
#test_syntax syntax_ressources/line_feed
#test_syntax syntax_ressources/line_feed2
#test_syntax syntax_ressources/multiple_space
#test_syntax syntax_ressources/space_and_tab
#test_syntax syntax_ressources/tab

#test_method $NAME_CONFIG POST /php/1.php
#test_method $NAME_CONFIG POST /php/2.php
#test_method $NAME_CONFIG POST /php/exemple.php
#test_method $NAME_CONFIG /php/info.php
#test_method $NAME_CONFIG POST /php/php.php -d arg1=O -d arg2=K -d arg3=!
#test_method $NAME_CONFIG POST /php/php.php -d arg1=ceci -d arg2=EST -d arg3=method -d arg4=POST
#test_method $NAME_CONFIG GET /php/php.php -G -d arg1=GET -d arg2=query -d arg3=STRING

stop_server

<< C
#PHP ERROR TEST
launch_server $PHP_ERROR_CONF

NAME_CONFIG=same_error_php.conf

test $NAME_CONFIG "/fdgfs" #404
test $NAME_CONFIG "/private/" #403
test $NAME_CONFIG "/secret/" #401
test $NAME_CONFIG "/html/cat.html" #405

stop_server

# C

<< C
TEST
#MULTIPLE SERVER NAME TEST
launch_multi_server $MULTIPLE_CONF

test "default" "/"
test "gif.conf" "/" -H "Host: gif"
test "jpeg.conf" "/" -H "Host: jpeg"
test "secret.conf" "/" -H "Host: secret"

stop_server



# TEST

delete_x_tmpdir DIR_TMP 1

#permission reset

#permission
chmod 755 srcs/private #because it s a directory
chmod 644 srcs/secret/mdp.html
chmod 644 srcs/spoiler/mdp.html