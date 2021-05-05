#include <message/response.hpp>

response::response(const request &req)
{
	std::string allow_method[] = {GET, HEAD, PUT, POST, CONNECT, OPTIONS, DELETE, PATCH, ""}; //will be replaced

	/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
	method_function header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request
	
	main_header(allow_method); /*add header_field which are present in all method*/

	/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
	first_line.status = (this->*header_field_function)(req);
	//if (first_line.status < 200 || first_line.status > 299)
	//	error_response(req);	
	first_line.status_string = find_status_string();
	first_line.version = req.get_version();
}

response::response(int status) : message()
{
	first_line.status = status;
	first_line.status_string = find_status_string();
	first_line.version = HTTP_VERSION;

	main_header(allow_method); /*add header_field which are present in all method*/
	body = ERROR_FILE;
	add_content_length();
}

response::~response(){};

