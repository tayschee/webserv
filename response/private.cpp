#include "request.hpp"

request::response::response_function request::response::get_method_function(const std::string &method, const std::string *allow_method)
{
	response_function	method_function[] = {&response::method_is_head(/*const std::string &, const std::map<std::string, std::string> &*/), &response::method_is_head, &response::method_is_head,
											&response::method_is_head, &response::method_is_head, &response::method_is_head,
											&response::method_is_head, &response::method_is_head, &response::method_is_head,
											&response::method_is_head};
	size_t i;

	for(i = 0; allow_method[i] != ""; i++)
	{
		if (allow_method[i] == method)
			break;
	}
	return(method_function[i]);
}


void			request::response::main_header(const std::string *allow_method)
{
	add_allow(allow_method); // add header field allow in header
	add_date();
	//add host
}