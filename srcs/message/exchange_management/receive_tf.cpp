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
	const size_t default_buf_size(10);
	char *buffer;
	ssize_t i;

	/*for now read the future size to read or read define size to optimise this it will be read define_size
	+ suppose the numer of char in an int*/
	if (this->buf_size == 0)
	{
		// std::cout << "===========  " << default_buf_size << " ============" << std::endl;
		buffer = new char[default_buf_size + 1];
		if ((i = read(socket, buffer, default_buf_size)) < 0)
		{
			delete[] buffer;
			return 500;
		}
		buffer[i] = 0;
		this->msg += std::string(buffer);
		delete[] buffer;
		if (i == 0)
			return -1;
		return(check(req));
	}
	else
	{
		// std::cout << "SIZE = " << buf_size << std::endl;
		buffer = new char[this->buf_size + 1];
		if ((i = read(socket, buffer, this->buf_size)) < 0)
			return 500;
		buffer[i] = 0;
		this->msg += std::string(buffer);
		// std::cout << "I = " << i << std::endl;
		delete[] buffer;
		if (i == 0)
			return -1;
		this->buf_size -= i;
		return 0;
	}
}

//check if object have reached end of read body ("0" + CRLF) and return 1 if it's true
int receive_management::receive_tf::check(message *req)
{
	size_t	i;
	size_t	CRLF_size(ft_strlen(CRLF));
	size_t	msg_size(msg.size());

	while ((i = msg.find(CRLF, pos)) != msg.npos) //check if there is CRLF
	{
		buf_size = ft_atoi_base<size_t>(msg.substr(pos), HEXADECIMAL_BASE); //store size of next_buffer
		// std::cout << "buf_size = " << buf_size << std::endl;
		if (buf_size == 0) //verify if it's end
		{
			if (pos == 0)
				msg.erase(pos); //erase 0\r\n + all after if there is
			else
				msg.erase(pos - CRLF_size); //erase 0\r\n + all after if there is + \r\n before
			req->body = msg; //complete req, it can be use
			return 1;
		}
		buf_size += CRLF_size;
		if (pos == 0)
			msg.erase(pos, (i + CRLF_size) - pos); //delete length of current chunk
		else
		{
			pos = pos - CRLF_size;
			msg.erase(pos, (i + CRLF_size) - pos); //delete length of current chunk + \r\n before, to have request without size
		}
		// std::cout << "buf_size2 = " << buf_size << std::endl;
		msg_size = msg.size();
		pos += buf_size;
		// std::cout << "message stocker = " << msg << std::endl;
		if (msg_size < pos) //if size of buffer not again read
		{
			// std::cout << "buf_size3 = " << buf_size << std::endl;
			// std::cout << "size stocker = " << msg_size << std::endl;
			// std::cout << "pos = " << pos << std::endl;

			buf_size = (pos - msg_size);
		
			// std::cout << "buf_size4 = " << buf_size << std::endl;
			
			return 0;
		}
		buf_size = 0;
		// std::cout << "buf_size5 = " << buf_size << std::endl;
	}
	return 0;
}

receive_management::receive_tf	*receive_management::receive_tf::clone() const
{
	receive_tf	*clone_obj;

	clone_obj = new(std::nothrow) receive_tf(*this);
	return clone_obj;
}