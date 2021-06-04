#include "message/exchange.hpp"
//#include "message/request.hpp"

/*this file contain function inside message::receive::header, it is use with receive::receive(...)
and serve to read particulary header, when it done this class must be change to an other internal_class child to read_body */

typedef  message::receive receive;

//header constructor destructor
receive::header::header(const size_t buf_size, const std::string &msg) : buf_size(buf_size), msg(msg) {}
receive::header::header(const header &x) : buf_size(x.buf_size), msg(x.msg) {}
receive::header::~header(){}

//header operator=
receive::header &receive::header::operator=(const header &x)
{
	buf_size = x.buf_size;
	msg = x.msg; //can maybe be change with assign parent of this to parent of x;
	
	return *this;
}


//Main Function

/*read header and when end is reached return 1 to change internal_receive struct and read body */
int		receive::header::receive(const int socket)
{
	char		*buffer = new char[buf_size + 1];
	ssize_t		i; //this just a long return type of read

	if ((i = read(socket, buffer, buf_size)) <= 0)
	{
		delete[] buffer;
		if (i == 0)
			return -1;
		else
			return 500; //replace by an exception
	}
	buffer[i] = 0;
	msg += buffer;
	delete[] buffer;

	return 0;
}

/*check if header is end*/
int			receive::header::check()
{
	size_t pos;

	pos = header_is_end(msg);
	if (pos != msg.npos)
		return 1;
	return 0;
}

/*must not be use*/
std::string							receive::header::get()
{
	return msg;
}

receive::body *receive::header::next_step()
{
	//SKIP HORIZONTAL_WHITE_SPACE
	receive::body *new_data;
	size_t header_end(header_is_end(msg));
	std::string header(msg.substr(0, header_end));
	size_t pos(header.find(TRANSFERT_ENCODING ":"));
	size_t end(msg.npos);

	if (pos != end)
	{
		new_data = new tf_body(msg, header_is_end(msg) + ft_strlen(SEPARATOR));
	}
	else if ((pos = header.find(CONTENT_LENGTH ":")) != end)
	{
		size_t buf_size(ft_atoi<size_t>(msg.substr(pos + ft_strlen(CONTENT_LENGTH ":") + 1)));
		size_t begin_body(header_end + ft_strlen(SEPARATOR));

		new_data = new cl_body(msg, buf_size, begin_body);
	}
	else
	{
		size_t begin_body(header_end + ft_strlen(SEPARATOR));

		new_data = new cl_body(msg, 0, begin_body);
	}
	return new_data;
}

receive::header	*receive::header::clone() const
{
	header *clone_obj;

	clone_obj = new receive::header(*this);
	return clone_obj;
}