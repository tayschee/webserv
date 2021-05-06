#include "message/message.hpp"

message::header_type message::get_header() const
{
	return (header);
}

std::string 		message::get_body() const
{
	return (body);
}