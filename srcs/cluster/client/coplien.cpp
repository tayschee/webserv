#include "client.hpp"

// defining all functions used for canonique mode.

// constructor used
client::client(int _fd, bool _listen, int _nb_pars) : fd(_fd), listen(_listen),
my_read(false), nb_pars(_nb_pars), rcv(fd, 4096), reset(false), alive(true), first(true),
msg(*(new std::string)), resp(*(new std::string)), header(*(new std::string)), my_cgi(NULL)
{
	h = 0;
	cgi_start = false;
	fdout = -1;
	file_body = tmpfile();
	file_out = NULL;
	fdbody = fileno(file_body);
	
	fcntl(fd, F_SETFL, O_NONBLOCK);
	fcntl(fdbody, F_SETFL, O_NONBLOCK);
    gettimeofday(&time, NULL);
}

client::client(const client& other) : rcv(other.rcv), msg(other.msg), resp(other.resp), header(other.header)// constructor by copy
{
	fd = other.fd;
	time = other.time;
	nb_pars = other.nb_pars;
	listen = other.listen;
	my_read = other.my_read;
}

client &client::operator=(const client& other) // assignation
{
	fd = other.fd;
	time = other.time;
	rcv = other.rcv;
	nb_pars = other.nb_pars;
	listen = other.listen;
	my_read = other.my_read;

	return *this;
}

client::~client()
{
	del();
	if (fd != -1)
		close(fd);
	delete(&msg);
	delete(&resp);
	delete(&header);

} // destructor