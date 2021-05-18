#include "message/request.hpp"
// defining all functions used for canonique mode.


/*
	0.Accept-Charsets					9.Host = 127.0.0.1 or localhost							
	1.Accept-Language = en				10.Last-Modified
	2.Allow								11.Location							
	3.Authorization						12.Referer								All informations which can contain header
	4.Content-Language					13.Retry-After
	5.Content-Length = nb char			14.Server = webserv
	6.Content-Location					15.Transfer-Encoding = extent of data?
	7.Content-Type = text/html			16.User-Agent
	8.Date =							17.WWW-Authenticate
*/

//do nothing wait read
request::request() : message() {}

//request::request(const request &x) : message(x) {}

request::request(const char *request_char) : message(), first_line()
{
	std::string request_str(request_char);
	
	parse_body(request_str); //fill body and body part of request_str is erased
	parse_header(request_str); //fill header + fill first_line
}

request request::operator=(const request &x)
{
	first_line = x.first_line;
	header = x.header;
	body = x.body;

	return (*this);
}

request::~request(){}