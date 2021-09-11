ps -f | grep fdadfas

if [[ $? ]]; then
	echo "exist"
else
	echo "doesnt exist"
fi