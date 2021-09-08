#source include.sh

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
	declare LS_TMP
	for ((i = 1; i < $2 + 1; i++)) do
	true #force first passage
	while [[ $? == 0 ]]; do
		export $1$i=$(cat /dev/urandom | env LC_CTYPE=C tr -cd 'a-f0-9' | head -c 32)
		LS_TMP=$1$i
		ls ${!LS_TMP} > /dev/null 2> /dev/null
	done

	TMP=${!LS_TMP}
	mkdir $TMP
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
