#include "message/exchange_management.hpp"

/*receive_cl struct, use to read body with Content-Length header, this struct is manipulated
by receive_management once header is read */

typedef message::receive_management receive_management;

//constructor destructor
receive_management::receive_cl::receive_cl() : internal_receive() {}
receive_management::receive_cl::receive_cl(const receive_cl &x) : internal_receive(x) {}

receive_management::receive_cl::receive_cl(internal_receive &x, size_t buf_size) : internal_receive(x)
{
	if (x.msg.size() < buf_size)
		this->buf_size = buf_size - x.msg.size();
	else
		this->buf_size = 0;
}

receive_management::receive_cl::~receive_cl(){}

//operator =
receive_management::receive_cl	&receive_management::receive_cl::operator=(const receive_cl &x)
{
	msg = x.msg;
	buf_size = x.buf_size;
	
	return *this;
}

//read body and when end is reached return 1 to delete object inside message::receive_management
int receive_management::receive_cl::receive(const int socket, message *req)
{
	char *buffer = new char[buf_size + 1];
	ssize_t i;

	if ((i = read(socket, buffer, this->buf_size)) < 0)
	{
		delete[] buffer;
		return 500;
	}
	buffer[i] = 0;
	this->msg += buffer;
	delete[] buffer;
	if (i == 0)
		return -1;
	return this->check(req);
}

//check if object have reached end of read body and return 1 if it's true
int receive_management::receive_cl::check(message *req)
{
	if (buf_size == 0)
	{
		req->body = this->msg;
		return 1;
	}
	return 0;
}

receive_management::receive_cl	*receive_management::receive_cl::clone() const
{
	receive_cl	*clone_obj;

	clone_obj = new receive_cl(*this);
	return clone_obj;
}