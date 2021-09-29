#include "message/exchange.hpp"
#include "message/request.hpp"
#include "message/response.hpp"

typedef message::send send;

send::internal::internal(int status, size_t buf_size, const std::string &msg) : status(status), buf_size(buf_size), msg(msg){}

send::internal::internal(const message &msg, size_t buf_size) : status(find_status_value(msg)), buf_size(buf_size), msg(msg.get()){}

send::internal::internal(const internal &x) : status(x.status), buf_size(x.buf_size), msg(x.msg) {}

send::internal::~internal(){};

send::internal &send::internal::operator=(const internal &x)
{
	status = x.status;
	buf_size = x.buf_size;
	msg = x.msg;

	return *this;
}

int		send::internal::find_status_value(const message &msg)
{
	if (dynamic_cast<const request *>(&msg) != NULL) //if msg is request
		return 1;
	else
		return dynamic_cast<const response &>(msg).get_status();
}

int	send::internal::get_status() const
{
	return status;
}

size_t send::internal::get_buf_size() const
{
	return buf_size;
}

const std::string &send::internal::get_msg() const
{
	return msg;
}