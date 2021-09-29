#include "message/message.hpp"

const message::header_type &message::get_header() const
{
	return (header);
}

const std::string 		&message::get_body() const
{
	return (body);
}