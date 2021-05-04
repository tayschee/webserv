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
int		receive_management::receive_header::receive(const int socket, message *req)
{
	char		*buffer = new(std::nothrow) char[buf_size + 1];
	ssize_t		i; //this just a long return type of read

	if ((i = read(socket, buffer, buf_size)) < 0)
		return -1; //throw exception
	buffer[i] = 0;
	msg += buffer;
	delete[] buffer;

	return check(req);
}

/*check if header is end*/
int			receive_management::receive_header::check(message *req)
{
	size_t pos;

	pos = msg.find(SEPARATOR);
	if (pos != msg.npos) //if there is double /r/n prepare receive_body
	{
		std::cout << "+++++++++++++++++++++\n" << msg << "+++++++++++++++++++++\n";
		req->parse_header(msg.substr(0, pos)); //fill header + request_line
		msg.erase(0, pos + ft_strlen(SEPARATOR));
		return 1; //header end
	}
	return 0; //header not end
}

receive_management::receive_header	*receive_management::receive_header::clone() const
{
	receive_header *clone_obj;

	clone_obj = new(std::nothrow) receive_management::receive_header(*this);

	return clone_obj;
}