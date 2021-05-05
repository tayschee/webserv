#include "../include/client.hpp"
#include <sys/time.h>
#include <fstream>      // std::ofstream
#include <unistd.h>

#include <iostream>

// defining all functions used for canonique mode.

client::client() : fd(-1), pars(), is_read(false), rcm(2000), rq()
{}

client::client(const client& other)
{
	fd = other.fd;
	time = other.time;
	rcm = other.rcm;
	rq = other.rq;
	pars = other.pars;
	listen = other.listen;
}

client &client::operator=(const client& other)
{
	fd = other.fd;
	time = other.time;
	rcm = other.rcm;
	rq = other.rq;
	pars = other.pars;
	listen = other.listen;
	return *this;
}

// defining "normal" constructor used to creat a new client.
// _fd is the new client fd.

client::client(int _fd, bool _listen, const parser &_pars) : fd(_fd), listen(_listen), pars(_pars), is_read(false), rcm(2000), rq()
{
    gettimeofday(&time, NULL);
}

client::~client(){}