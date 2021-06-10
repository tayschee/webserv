#include "cgi.hpp"

cgi::cgi(const request &req, const parser &pars, std::string &body, const std::string &path)
{
	body = exec(init_env(req, pars, path), req, pars, path);
	if (body[0] == '5')
		return;
	if (body.find(SEPARATOR) != body.npos)
		body.erase(0, body.find(SEPARATOR) + 4);
	// body.insert(0, "28E\r\n");
	// body += "\r\n\r\n0\r\n\r\n";

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