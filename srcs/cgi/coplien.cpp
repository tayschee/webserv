#include "cgi.hpp"

cgi::cgi(const request &req, const parser &pars, const std::string &path, int &fdin, int &fdout)
{
	std::cout << "DANS LE COPLIEN DE CGI" << std::endl;
	exec(init_env(req, pars, path, fdin), pars, path, fdin, fdout);

	// if (body[0] == '5')
	// 	return;
	// if (body.find(SEPARATOR) != body.npos)
	// 	body.erase(0, body.find(SEPARATOR) + 4);
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

cgi::~cgi(){}