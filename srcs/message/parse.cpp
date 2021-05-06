# include "message/message.hpp"

/*store body part of request inside request::body and erase body_part in request_str*/
void		message::parse_body(std::string &request_str)
{
	std::string	sep(SEPARATOR);
	size_t pos(request_str.find(sep));

	if (pos != request_str.npos)
		body = request_str.substr(pos + sep.size()); /* if body exists, body = all after "\r\n\r\n" */

	request_str.erase(pos);
}