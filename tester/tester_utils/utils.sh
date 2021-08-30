get_response()
{
	declare HEADER=TMP"$1"
	declare BODY=TMP"$(expr "$1" + 1)"
	declare EVERYTHING=TMP"$(expr "$1" + 2)"
	declare IP="$2"
	declare PORT="$3"

	echo 1 $1
	echo 2 $2
	echo 3 $3
	echo 4 $4
	echo 5 $5
	echo 6 $6
	echo 7 $7
	echo 8 $8
	curl -sSiX "$4" "${@:6}" $IP:$PORT$5 > ${!EVERYTHING}
	cat ${!EVERYTHING} | grep -A-1 "\r\n\r\n" > ${!BODY}
	cat ${!EVERYTHING} | grep -B-1 "\r\n\r\n" > ${!HEADER}
	# echo "" > ${!EVERYTHING}
}

print_diff()
{
	export HEADER_NGINX=TMP$1
	export BODY_NGINX=TMP$(expr "$1" + 1)
	export HEADER_WEBSERV=TMP"$2"
	export BODY_WEBSERV=TMP$(expr "$2" + 1)

	export HEADER_DIFF=TMP"$3"
	export BODY_DIFF=TMP$(expr "$3" + 1)

	export HEADER_DIFF_BOOL
	export BODY_DIFF_BOOL

	echo 11 $1
	echo 12 $2
	echo 13 $3
	echo 14 $4
	echo 15 $5
	echo 16 $6
	echo 17 $7
	echo 18 $8
	diff -y --suppress-common-lines -a ${!HEADER_NGINX} ${!HEADER_WEBSERV} > ${!HEADER_DIFF}
	HEADER_DIFF_BOOL=$?
	diff ${!BODY_NGINX} ${!BODY_WEBSERV} > ${!BODY_DIFF}
	BODY_DIFF_BOOL=$?
	echo path body : ${!BODY_DIFF}
	cat ${!BODY_DIFF}
	echo path header : ${!HEADER_DIFF}
	if [[ ${!BODY_DIFF} == ${!HEADER_DIFF} ]]; then
		echo path equal
	else
		echo not equal
	fi
	cat ${!HEADER_DIFF}
    if [[ $HEADER_DIFF_BOOL ]] || [[ $BODY_DIFF_BOOL ]]; then
        echo -e "---------------------- " "${@:4}" " ------------------------\n" >> $OUTPUT
        echo -e "${@:4}" ": CHECK " $OUTPUT
    else
        echo -e "${@:4}" ": OK"
    fi
	if [[ HEADER_DIFF_BOOL != 0 ]]; then
        cat ${!HEADER_DIFF} >> $OUTPUT
	fi
	if [[ BODY_DIFF_BOOL != 0 ]]; then
        cat ${!BODY_DIFF} >> $OUTPUT
    fi

	echo "" > ${!HEADER_DIFF}
	echo "" > ${!BODY_DIFF}
}