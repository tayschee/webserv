#include "message/request.hpp"

void	request::request_line::clear()
{
	method.clear();
	uri.clear();
	version.clear();
}

void	request::clear()
{
	first_line.clear();
	header.clear();
	body.clear();
}