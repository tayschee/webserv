#include "message/exchange.hpp"

/*cl_body struct, use to read body with Content-Length header, this struct is manipulated
by receive once header is read */

typedef message::receive receive;

//constructor destructor
receive::cl_body::cl_body(const std::string &msg, const size_t buf_size, const size_t body_begin) : body(msg, buf_size)
{
	if (buf_size < msg.size() - body_begin)
	{
		this->pos = body_begin + this->buf_size;
		this->buf_size = 0;
	}
	else
	{
		this->pos = body_begin + buf_size;
		this->buf_size -= (msg.size() - body_begin);
	}
}
receive::cl_body::cl_body(const cl_body &x) : body(x) {}

receive::cl_body::~cl_body(){}

//operator =
receive::cl_body	&receive::cl_body::operator=(const cl_body &x)
{
	msg = x.msg;
	buf_size = x.buf_size;
	pos = x.pos;
	
	return *this;
}

//read body and when end is reached return 1 to delete object inside message::receive
int receive::cl_body::receive(const int socket)
{
	char *buffer = new char[buf_size + 1];
	ssize_t i;

	if ((i = read(socket, buffer, this->buf_size)) <= 0)
	{
		msg.clear();
		delete[] buffer;
		buffer = NULL;
		return -1;
	}
	buffer[i] = 0;
	this->msg += buffer;
	memset(buffer, 0, buf_size + 1);
	delete[] buffer;
	buffer = NULL;
	buf_size -= i;
	return 0;
}

//check if object have reached end of read body and return 1 if it's true
int receive::cl_body::check()
{
	if (buf_size == 0)
		return 1;
	return 0;
}

receive::cl_body	*receive::cl_body::clone() const
{
	cl_body	*clone_obj;

	clone_obj = new cl_body(*this);
	return clone_obj;
}

std::string                         receive::cl_body::get_buffer()
{
    size_t del_pos = msg.size() < pos ? msg.size() : pos;
    std::string buffer(msg.substr(0, del_pos));
    
    pos -= del_pos;
    msg.erase(0, del_pos);
    return buffer;
}