generate_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
	true #force first passage
	while [[ $? == 0 ]]; do
		export $1$i=$(cat /dev/urandom | env LC_CTYPE=C tr -cd 'a-f0-9' | head -c 32)
		declare TMP=$1$i
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

delete_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
		declare TMP=$1$i
		rm -f ${!TMP}
	done
}

clear_x_tmpfile()
{
	for ((i = 1; i < $2 + 1; i++)) do
		declare TMP=$1$i
		echo "" > ${!TMP}
	done
}

generate_x_tmpfile TMP 9
declare TMP=$TMP1
echo abc > $TMP
sleep 10
clear_x_tmpfile TMP 9
sleep 5
delete_x_tmpfile TMP 9