#include <request.hpp>


request::response::response(/*std::string (&method)[3], header_type &header, std::string &body*/)
{
	/*array of allow_method, if you add one here you MUST add function in method_function inside get_method_function,
	"" is for end of array*/
	const std::string	allow_method[] = {"GET", "HEAD", "POST", "PUT", "DELETE",
										"CONNECT", "OPTIONS", "TRACE", "PATCH", ""};

	/*without typedef reponse_function f write it, typedef int (response::*f)(void).
	This is pointeur to function*/
	response_function header_field_function;

	//get pointeur on function associated with the method of request
	header_field_function = get_method_function("HEAD", allow_method);
	
	main_header(allow_method); /*add header_field which are present in all method*/

	(this->*header_field_function)("", std::map<std::string, std::string>()); /*call pointer to member function this is exactly like that
										we must call it ALL bracket are neccessary there is no other way*/
}

request::response::~response(){};


const std::string	request::response::message() const
{
	std::string msg;
	
	//"reponse number" to string
	msg += header_to_string();
	//body to string
	return msg;
}

