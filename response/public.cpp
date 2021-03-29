#include <request.hpp>


request::response::response(/*std::string (&method)[3], header_type &header, std::string &body*/)
{
	/*array of allow_method if you add one here you MUST add function in method_function inside get_method_function,
	"" is for end of array*/
	const std::string	allow_method[] = {"GET", "HEAD", "POST", "PUT", "DELETE",
											"CONNECT", "OPTIONS", "TRACE", "PATCH", ""};

	response_function f; /*without typedef reponse_function f write it typedef int (response::*f)(void)*/
	
	f = get_method_function("HEAD", allow_method);
	
	main_header(allow_method);

	(this->*f)(); /*call pointer to member function this is exactly like that
					we must call it ALL bracket are neccessary there is no other way*/
}

request::response::~response(){};

