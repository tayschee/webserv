#include "message/exchange_management.hpp"

/*receive_tf struct, use to read body with Content-Length header, this struct is manipulated
by receive_management once header is read */

typedef message::receive_management receive_management;

//constructor destructor
receive_management::receive_tf::receive_tf() : internal_receive(), pos(0) {}
receive_management::receive_tf::receive_tf(const std::string &msg) : internal_receive(msg, 0), pos(0) {}
receive_management::receive_tf::receive_tf(const receive_tf &x) : internal_receive(x), pos(x.pos) {}
receive_management::receive_tf::~receive_tf(){}

//operator =
receive_management::receive_tf	&receive_management::receive_tf::operator=(const receive_tf &x)
{
	msg = x.msg;
	buf_size = x.buf_size;
	pos = x.pos;
	
	return *this;
}

//read body and when end is reached return 1 to delete object inside message::receive_management
int receive_management::receive_tf::receive(const int socket, message *req)
{
	const size_t default_buf_size(1000);
	char *buffer;
	ssize_t i;

	/*if (this->buf_size == 0)
	{*/
		std::cout << "buf1\n";
		buffer = new(std::nothrow) char[default_buf_size + 1];
		if ((i = read(socket, buffer, default_buf_size)) < 0)
			return -1;
		std::cout << "i :" <<  i << "\n";
		buffer[i] = 0;
		this->msg += std::string(buffer);
		delete[] buffer;
		//std::cout << this->buf_size << "\n";
		return(check(req));
	/*}
	else
	{
		std::cout << "buf2\n";
		buffer = new(std::nothrow) char[this->buf_size + 1];
		if ((i = read(socket, buffer, this->buf_size)) < 0)
			return -1;
		buffer[i] = 0;
		delete[] buffer;
		this->buf_size -= i;
		std::cout << "################\n" << this->msg << "\n#################\n";
		//std::cout << this->buf_size << "\n";
		return(check(req));
	}*/
}

//check if object have reached end of read body and return 1 if it's true
int receive_management::receive_tf::check(message *req)
{
	size_t	i;
	size_t	CRLF_size(ft_strlen(CRLF));
	size_t	msg_size(msg.size());

	while ((i = msg.find(CRLF, pos)) != msg.npos && msg_size > i + CRLF_size)
	{
		std::cout << "----------------\n" << msg.substr(pos, i - pos) << "\n---------------\n";
		std::cout << "----------------\n" << msg << "\n---------------\n";
		std::cout << "size : " << msg.substr(pos, i - pos).size() << "\n";
		std::cout << "pos :" << pos << "\n";
		std::cout << "i :" << i << "\n";
		buf_size = ft_atoi<size_t>(msg.substr(pos, i));
		std::cout << "abs buf size : " << buf_size << "\n";
		if (buf_size == 0)
		{
			std::cout << "HERE !!!!\n";
		//	msg.erase(pos); //delete all after unnecssary
			req->body = msg; //complete req, it can be use
			return 1;
		}
		//else
		//	msg.erase(pos, i + CRLF_size); //delete length of current chunk
		buf_size += CRLF_size;
		msg_size = msg.size();
		pos = i + CRLF_size + buf_size; //set pos to next value to read
		std::cout << "pos :" << pos << "\n";
		if (msg_size < pos) //stop if size < pos
		{
			std::cout << "ciao1\n";
			buf_size = buf_size - (pos - msg_size);
			std::cout << "buf_size : " << buf_size << "\n";
			return 0;
		}
		buf_size = 0;
	}
	std::cout << "ciao2\n";
	return 0;
}

receive_management::receive_tf	*receive_management::receive_tf::clone() const
{
	receive_tf	*clone_obj;

	clone_obj = new(std::nothrow) receive_tf(*this);
	return clone_obj;
}