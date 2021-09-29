# include "message/message.hpp"

/*store body part of request inside request::body and erase body_part in request_str*/
void		message::parse_body(std::string &request_str)
{
	request_str.erase(0, skip(request_str, CRLF));

	std::string	sep(SEPARATOR);
	size_t pos(request_str.find(sep));

	if (pos != request_str.npos)
		body = request_str.substr(pos + sep.size()); /* if body exists, body = all after "\r\n\r\n" */

	request_str.erase(pos);
}

/*function to know if header is complete true if yes and false else */
size_t			message::header_is_end(const std::string header_str)
{
	size_t pos;

	//find separator but ignore if there is SEPARATOR before start of message
	pos = header_str.find(SEPARATOR, skip(header_str, CRLF));
	
	return pos;
}