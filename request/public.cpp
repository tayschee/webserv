#include <request.hpp>

/* wait instruction */
//void		request::wait(){}

/*execute method store in method[CMD]*/
std::string	request::send_response() const
{
	response	resp(method, header, body);

	return (resp.message());
}