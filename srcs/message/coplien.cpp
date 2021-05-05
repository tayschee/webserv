#include "message/message.hpp"

message::message() : header(), body(){}

message::message(header_type &header, std::string &body) : header(header), body(body){}

message::message(message &x) : header(x.header), body(x.body){}

/*message message::operator=(message &x)
{
	header = x.header;
	body = x.body;

	return *this;
}*/

message::~message(){}