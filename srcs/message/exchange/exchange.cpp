#include "message/exchange.hpp"

//constructor and destructor of class exchange inside message it is parent of receive and send
typedef message::exchange exchange;

/*parent class to receive and send content with child */

//constructor destructor
exchange::exchange(int fd) : fd(fd){

}
exchange::exchange(const exchange &x) : fd(x.fd){}
exchange::~exchange(){

}

//operator =
exchange &exchange::operator=(const exchange &x)
{ 
	fd = x.fd;
	return *this;
}

