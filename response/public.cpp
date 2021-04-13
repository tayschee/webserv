#include <request.hpp>


request::response::response(const std::string (&method)[3], const header_type &req_head, const std::string &body)
{
	/*array of allow_method, if you add one here you MUST add function in method_function inside get_method_function,
	"" is for end of array, method are define inside define.hpp*/
	const std::string	allow_method[] = {GET, HEAD, POST, PUT, DELETE,
										CONNECT, OPTIONS, TRACE, PATCH, ""};

	/*without typedef reponse_function f write it, typedef int (response::*f)(void).*/
	response_function header_field_function; //this is pointer to function

	//get pointeur on function associated with the method of request
	header_field_function = get_method_function(method[METHOD], allow_method);
	
	main_header(allow_method); /*add header_field which are present in all method*/

	/*call pointer to member function this is exactly like that we must call it,
	 ALL bracket are neccessary there is no other way*/
	status = (this->*header_field_function)("cat.html", req_head, body);

	(void)body; //for now do nothing
	version = method[VERSION];
}

request::response::~response(){};


const std::string	request::response::message() const
{
	std::string msg;
	
	msg = header_first_line();
	msg += header_to_string();
	msg += CRLF;
	msg += body;

	return msg;
}

