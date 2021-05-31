#include "message/exchange_management.hpp"

typedef message::send_management send_management;

send_management::send_management() : exchange_management(), msg(""), buf_size(0) {}
send_management::send_management(std::string msg, size_t buf_size) : exchange_management(), msg(msg), buf_size(buf_size){}
send_management::send_management(message *msg) : exchange_management(), msg(msg->get()), buf_size(this->msg.size()){}
send_management::send_management(message *msg, size_t buf_size) : exchange_management(), msg(msg->get()), buf_size(buf_size){}

send_management &send_management::operator=(const send_management &x)
{
	msg = x.msg;
	buf_size = x.buf_size;

	return *this;
}
