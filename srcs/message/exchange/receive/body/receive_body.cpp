#include "message/exchange.hpp"

typedef  message::receive receive;

//header constructor destructor
receive::body::body(const std::string &msg, const size_t buf_size, const size_t pos) : receive::header(buf_size, msg), pos(pos) {}
receive::body::body(const body &x) : receive::header(x), pos(x.pos) {}
receive::body::~body(){}

//body operator=
receive::body &receive::body::operator=(const body &x)
{
	buf_size = buf_size;
	msg = x.msg; //can maybe be change with assign parent of this to parent of x;
	pos = x.pos;
	
	return *this;
}

std::string 						receive::body::get_buffer()
{
	std::string buffer(msg.substr(0, pos));
	msg.erase(0, pos);
	return buffer;
}

std::string 						receive::body::get_header_buffer()
{
	size_t pos(header_is_end(msg));
	std::string header(msg.substr(0, pos));

	pos = pos + ft_strlen(SEPARATOR);
	msg.erase(0, pos);
	this->pos -= pos;

	return header;
}

receive::header		*receive::body::previous_step(const size_t buf_size) const
{
	//std::cout << "msg : " << msg << "\n";
	//std::cout << "pos : " << pos << "\n";
	//receive::header *new_data = new header(buf_size, msg.substr(pos));
	receive::header *new_data = new header(buf_size, msg);

	return new_data;
}