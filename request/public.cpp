#include <request.hpp>

/*execute method store in method[CMD]*/
std::string	request::send_response() const
{
	response	resp(method, header, body);

	return (resp.message());
}