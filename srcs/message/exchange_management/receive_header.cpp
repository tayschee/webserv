#include "message/exchange_management.hpp"

/*this file contain function inside message::receive_management::receive_header, it is use with receive_management::receive(...)
and serve to read particulary header, when it done this class must be change to an other internal_class child to read_body */

typedef  message::receive_management receive_management;

//receive_header constructor destructor
receive_management::receive_header::receive_header() : internal_receive(){}
receive_management::receive_header::receive_header(size_t buf_size) : internal_receive(buf_size){}
receive_management::receive_header::receive_header(const receive_header &x) : internal_receive(x){}
receive_management::receive_header::~receive_header(){}

//receive_header operator=
receive_management::receive_header &receive_management::receive_header::operator=(const receive_header &x)
{
	msg = x.msg; //can maybe be change with assign parent of this to parent of x;
	buf_size = x.buf_size;
	
	return *this;
}

/*read header and when end is reached return 1 to change internal_receive struct and read body */
int		receive_management::receive_header::receive(const int socket)
{
	char		*buffer = new char[buf_size + 1];
	ssize_t		i; //this just a long return type of read

	if ((i = read(socket, buffer, buf_size)) < 0)
	{
		delete[] buffer;
		return 500; //throw exception
	}
	buffer[i] = 0;
	msg += buffer;
	delete[] buffer;
	if (i == 0)
		return -1;
	//return check();
	return 0;
}

/*check if header is end*/
int			receive_management::receive_header::check()
{
	size_t pos;

	pos = header_is_end(msg);
	if (pos != msg.npos)
		return 1;
	return 0;
}

receive_management::internal_receive *receive_management::receive_header::next_step() const
{
	receive_management::internal_receive *new_data;
	size_t pos(msg.find(TRANSFERT_ENCODING ":"));
	size_t end(msg.npos);

	if (pos != end)
	{
		new_data = new receive_tf(msg, msg.find(SEPARATOR) + ft_strlen(SEPARATOR));
		std::cout << "transfert encoding nice detected\n";
	}
	else
	{
		new_data = NULL;
	}
	/*else if ((pos = msg.find(CONTENT_LENGTH ":")) != end)
	{
		size_t buf_size(ft_atoi<size_t>(msg.substr(pos + ft_strlen(CONTENT_LENGTH ":"))));
		new_data = new receive_cl(msg, buf_size - pos));
	}
	else
	{
		new_data = new receive_cl(msg, 0);
	}*/
	return new_data;
}

receive_management::receive_header	*receive_management::receive_header::clone() const
{
	receive_header *clone_obj;

	clone_obj = new receive_management::receive_header(*this);

	return clone_obj;
}