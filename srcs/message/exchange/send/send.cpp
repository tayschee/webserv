#include "message/exchange.hpp"

typedef message::send send;

send::send(const std::string &msg, const int status, const size_t buf_size, const int fd) :
exchange(fd), queue(std::queue<internal>::container_type(1, internal(status, buf_size, msg))) {}

send::send(const message &msg, const size_t buf_size, const int fd) :
exchange(fd), queue(std::queue<internal>::container_type(1, internal(find_status_value(msg), buf_size, msg.get()))) {}

send::send(const send &x) : exchange(fd), queue(x.queue) {}

send::~send(){}

send &send::operator=(const send &x)
{
	queue = x.queue;
	fd = x.fd;

	return *this;
}

/*Send to fd_dest a message return -1 if there is an error 0 if msg is not completly send and a number > 0 when he is sent.
If this a request this always 1, else this a status value*/
int	send::operator()()
{
	ssize_t res;
	internal &data(queue.front());

	res = write(fd, data.get_msg().c_str(), data.get_buf_size());
	if (res == -1 || res == 0)
		return -1;
	res = data.get_status();
	queue.pop(); //i dont know if write can be okay for n character then miss
	return res;
}

void 				send::push(const message &msg, const size_t buf_size)
{
	queue.push(internal(msg, buf_size));
}
