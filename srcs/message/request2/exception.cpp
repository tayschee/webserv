#include "message/request.hpp"

typedef request::exception exception;

exception::exception(int status, const std::string &method) : 
std::exception(), status(status), method(method) {}

exception::~exception() throw() {}

exception::exception(const exception &x) : std::exception(), status(x.status), method(x.method) {}

exception &exception::operator=(const exception &x)
{
	status = x.status;
	method = x.method;

	return *this;
}

const std::string	&exception::get_method() const
{
	return (method);
}

int					exception::get_status() const
{
	return (status);
}

const char *exception::what() const throw()
{
	return ("request::exception"); //that could be segfault ????
}