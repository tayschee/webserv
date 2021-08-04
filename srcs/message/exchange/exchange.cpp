#include "message/exchange.hpp"

#include <iostream>

//constructor and destructor of class exchange inside message it is parent of receive and send
typedef message::exchange exchange;

/*parent class to receive and send content with child */

//constructor destructor
exchange::exchange(int fd) : fd(fd){
	// std::cout << "JE SUIS DANS LE CONSTRUCTEUR DE EXANGE" << std::endl;

}
exchange::exchange(const exchange &x) : fd(x.fd){}
exchange::~exchange(){
	// std::cout << "JE SUIS DANS LE DESTRUCTEUR DE EXANGE" << std::endl;

}

//operator =
exchange &exchange::operator=(const exchange &x)
{ 
	fd = x.fd;
	return *this;
}

