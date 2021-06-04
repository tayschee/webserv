#include "message/exchange_management.hpp"
#include "message/request.hpp"
#include "message/response.hpp"

typedef message::send_management send_management;

send_management::send_management() : exchange_management(), data_list(), dest_fd(0){}
send_management::send_management(const message &msg, const size_t buf_size, const int dest_fd) :
exchange_management(), data_list(1, internal_send(msg, buf_size)), dest_fd(dest_fd) {}
send_management::send_management(const send_management &x) : exchange_management(), data_list(x.data_list), dest_fd(x.dest_fd){}

send_management::~send_management()
{
	data_list.clear();
}

send_management &send_management::operator=(const send_management &x)
{
	data_list = x.data_list;
	dest_fd = x.dest_fd;

	return *this;
}

/*send to fd_dest a message return -1 if there is an error 0 if msg is not completly send and a number > 0 when he is sent.
If this a request this always 1, else this a status value*/
int	send_management::send()
{
	ssize_t res;
	std::list<internal_send>::iterator it(data_list.begin());
	size_t size(it->msg.size() < it->buf_size ? it->msg.size() : it->buf_size);
	

	res = write(dest_fd, it->msg.c_str(), size);
	if (res == -1)
		return -1;
	it->msg.erase(0, size);
	if (it->msg.size() == 0)
	{
		data_list.erase(it);
		return it->status;
	}
	else
		return 0;
}

void 				send_management::add_element_to_send(const message &msg, const size_t buf_size)
{
	data_list.push_back(internal_send(msg, buf_size));
}

//internal send

send_management::internal_send::internal_send(int status, size_t buf_size, const std::string &msg) :
internal_exchange(), status(status), buf_size(buf_size), msg(msg){}

send_management::internal_send::internal_send(const message &msg, size_t buf_size) :
internal_exchange(), status(find_status_value(msg)), buf_size(buf_size), msg(msg.get()){}

send_management::internal_send::internal_send(const internal_send &x) :
internal_exchange(), status(x.status), buf_size(x.buf_size), msg(x.msg) {}

send_management::internal_send::~internal_send(){};

send_management::internal_send &send_management::internal_send::operator=(const internal_send &x)
{
	status = x.status;
	buf_size = x.buf_size;
	msg = x.msg;

	return *this;
}

int		send_management::internal_send::find_status_value(const message &msg)
{
	if (dynamic_cast<const request *>(&msg) != NULL) //if msg is request
		return 1;
	else
		return dynamic_cast<const response &>(msg).get_status();
}
