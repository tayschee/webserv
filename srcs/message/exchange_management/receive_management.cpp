#include "message/exchange_management.hpp"

typedef message::receive_management receive_management;

/* receive_management parent class of other receive class,
it used to receive header and body through is internal class and it variable which change type according to needs
(read header, cl_body, tf_body), it's also child class of exchange_management. It is used with function receive of request */

//receive_mangement constructor destructor
receive_management::receive_management() : exchange_management(), data(NULL), buf_size(1) {}

receive_management::receive_management(size_t buf_size) : exchange_management(), data(new receive_header(buf_size)), buf_size(buf_size) {}

receive_management::receive_management(const receive_management &x) : 
exchange_management(), data(x.clone()), buf_size(x.buf_size){}

receive_management::~receive_management()
{
	clear();
}


//receive_mangement operator =
receive_management &receive_management::operator=(const receive_management &x)
{
	data = x.clone();
	buf_size = x.buf_size;
	return *this;
}

/* nested classes of receive_management to optimise reading,
internal_class, pure virtual parent class of receive_header, receive_cl, exist to change son class inside receive_management */

//internal_receive constructor and desctuctor
receive_management::internal_receive::internal_receive() : internal_exchange(), buf_size(0) {}
receive_management::internal_receive::internal_receive(size_t buf_size) : internal_exchange(), buf_size(buf_size) {}
receive_management::internal_receive::internal_receive(const std::string &msg, const size_t buf_size) : internal_exchange(msg), buf_size(buf_size) {}
receive_management::internal_receive::internal_receive(const internal_receive &x) : internal_exchange(x), buf_size(x.buf_size){}
receive_management::internal_receive::~internal_receive(){}

//internal_receive operator =
receive_management::internal_receive &receive_management::internal_receive::operator=(const internal_receive &x)
{
	msg = x.msg; //can maybe be change with assign parent of this to parent of x;
	buf_size = x.buf_size;
	
	return *this;
}