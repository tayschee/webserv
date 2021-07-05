#include "message/response.hpp"
#include <iostream>

response::response() : message(), first_time(1)
{}

response::response(const request &req, const std::vector<parser::address_conf>::const_iterator &pars_list) : message(), first_time(1)
{
	first_line.status = 400;
	parser::address_conf::const_iterator pars_it = find_parser(pars_list, req);
	if (pars_list->end() == pars_it || (first_line.status = req.validity(*pars_it)) != 0)
	{
		main_header();
		first_line.status = error_response(first_line.status, req);
	}
	else
	{
		const parser &pars(*pars_it);
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(PARSER_ACCEPT)->second);

		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
		method_function method = find_method_function(req.get_method(), allow_method); //give function associate with request

		main_header(allow_method); /*add header_field which are present in all method*/
		first_line.status = generate_response(path_info, pars, req, method);
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

