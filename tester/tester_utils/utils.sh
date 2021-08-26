get_response()
{
	declare HEADER=TMP"$1"
	declare BODY=TMP"$(expr "$1" + 1)"
	declare IP="$2"
	declare PORT="$3"

	echo 4 $4
	echo 5 $5
	echo 6 $6
	curl -sSIX "$4" "${@:6}" $IP:$PORT$5 > ${!HEADER}
	curl -sSX "$4" "${@:6}" $IP:$PORT$5 > ${!BODY}
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

	echo body diff : ${BODY_DIFF}
	echo body diff : ${!BODY_DIFF}
	diff -y --suppress-common-lines -a ${!HEADER_NGINX} ${!HEADER_WEBSERV} > ${!HEADER_DIFF}
	HEADER_DIFF_BOOL=$?
	diff ${!BODY_NGINX} ${!BODY_WEBSERV} > ${!BODY_DIFF}
	BODY_DIFF_BOOL=$?
	echo $BODY_DIFF
	echo $HEADER_DIFF
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