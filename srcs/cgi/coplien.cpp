#include "cgi.hpp"

cgi::cgi(const request &req, const parser &pars, const std::string &path, int &fdin, int &fdout, int fd)
{
	exec(init_env(req, pars, path, fdin), pars, path, fdin, fdout, fd);
}

cgi::cgi(const cgi& other)
{
	(void)other;
}

cgi		&cgi::operator=(const cgi& other)
{
	(void)other;
	return *this;
}

int			cgi::get_pid()
{
	return my_pid;
}

void		cgi::reset_pid()
{
	my_pid = 0;
}

cgi::~cgi(){}