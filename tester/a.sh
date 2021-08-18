NGINX_IP=127.0.0.1
NGINX_PORT=80

trap test SIGINT

test() {
	echo oui
	exit 0
}

sleep 12