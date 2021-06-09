#include "message/response.hpp"
#include <iostream>

response::response(const request &req, const parser &pars) : message()
{
	std::cout << "response\n";
	if (req.validity(pars) != 0)
	{
		std::cout << "here\n";
		first_line.status = error_response(400, req, pars);
		first_line.status_string = find_status_string();
	}
	else
	{
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		//std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
		std::vector<std::string> allow_method(1, GET);
		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
		std::cout << "ok2\n";
		method_function header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request
		std::cout << "ok3\n";
		main_header(allow_method); /*add header_field which are present in all method*/
		/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
		if (!is_redirect(path_info, pars))
		{
		//	std::cout << "ok4\n";
			first_line.status = (this->*header_field_function)(req.get_uri(), req, pars);
			std::cout << "ok5\n";
			if (first_line.status > 299)
			{
				std::cout << "ok6\n";
				first_line.status = error_response(first_line.status, req, pars);
				std::cout << "ok7\n";
			}
		}
		
		first_line.status_string = find_status_string();
		first_line.version = req.get_version();
	}
}

response::response(const request::exception except, const parser &pars) : message(), first_line()
{
	first_line.status = except.get_status();
	first_line.version = HTTP_VERSION;

	request small_size_request(except);

	main_header();
	first_line.status = error_response(first_line.status, small_size_request, pars);
	first_line.status_string = find_status_string();
	first_line.version = HTTP_VERSION;
}

response::~response(){};

