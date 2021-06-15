#include "message/response.hpp"
#include <iostream>

response::response() : message(), first_time(1)
{}

response::response(const request &req, const parser::address_conf *pars_vec) : message(), first_time(1)
{
	const parser *pars_point = find_parser(pars_vec, req);
	if (req.validity(pars_point) != 0)
	{
		main_header();
		//first_line.status = error_response(first_line.status, req, pars);
	}
	else
	{
		const parser &pars(*pars_point);

		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(PARSER_ACCEPT)->second);

		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
		method_function header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request


		main_header(allow_method); /*add header_field which are present in all method*/
		/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
		//if (path_info.find("maxBody") != path_info.end() && req.get_body().size() > ft_atoi<unsigned long>(path_info.find("maxBody")->second[0]))
		//{
		//	first_line.status = 413;
		//	get_code(pars);
		//}		
		if (!is_redirect(path_info, pars))
		{
			// std::cout << "ok4\n";
			first_line.status = (this->*header_field_function)(req.get_uri(), req, pars);
			// std::cout << "ok5\n";
			if (first_line.status > 299)
			{
				// std::cout << "ok6\n";
				get_code(pars);
				//first_line.status = error_response(first_line.status, req, pars);
				// std::cout << "ok7\n";
			}
		}	
	}

	first_line.status_string = find_status_string();
	first_line.version = req.get_version();
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

