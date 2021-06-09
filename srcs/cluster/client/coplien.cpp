#include "client.hpp"

// defining all functions used for canonique mode.

client::client() : fd(-1), read(false), pars(), rcv(), req() // constructor by default
{}

// constructor used
client::client(int _fd, bool _listen, const parser &_pars) : fd(_fd), listen(_listen), read(false), pars(_pars), rcv(fd, 10), req()
{
    gettimeofday(&time, NULL);
}

client::client(int _fd, bool _listen, const client &other) : fd(_fd), listen(_listen), read(false), pars(other.pars), rcv(fd, 10), req()
{
    gettimeofday(&time, NULL);
}

client::client(const client& other) // constructor by copy
{
	fd = other.fd;
	time = other.time;
	rcv = other.rcv;
	req = other.req;
	pars = other.pars;
	listen = other.listen;
	read = other.read;
}

client &client::operator=(const client& other) // assignation
{
	fd = other.fd;
	time = other.time;
	rcv = other.rcv;
	req = other.req;
	pars = other.pars;
	listen = other.listen;
	read = other.read;
	return *this;
}

client::~client(){} // destructor