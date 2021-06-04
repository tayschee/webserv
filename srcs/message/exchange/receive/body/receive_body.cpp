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