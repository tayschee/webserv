#include "message/response.hpp"
#include <iostream>

response::response() : message()
{
}

response::response(const request &req, const std::vector<parser::address_conf>::const_iterator &pars_list) : message()
{
	std::cout << "ok\n";
	first_line.status = 400;
	const parser::address_conf::const_iterator pars_it = find_parser(pars_list, req);
	std::cout << &pars_it << "\n";
	std::cout << &pars_list << "\n";
	if (pars_it == pars_list->end() || (first_line.status = req.validity()) != 200)
	{
		main_header();
		first_line.status = error_response(first_line.status, req);
	}
	else
	{
		const parser &pars(*pars_it);
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(PARSER_ACCEPT)->second); //no protect
		std::string path = find_path(pars.get_block(BLOCK_LOCATION , req.get_uri()), req.get_uri(), req);
		method_function method;

		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
		if (is_cgi(get_extension(path), pars, req.get_method()))
			method = existing_method.find(POST)->second; //give function associate with request
		else
			method = find_method_function(req.get_method(), allow_method); //give function associate with request

		main_header(allow_method); /*add header_field which are present in all method*/
		first_line.status = generate_response(path_info, pars, req, method);
	}
	first_line.status_string = find_status_string(first_line.status);
	first_line.version = req.get_version();
	std::cout << "seems ok\n";
	//std::cout << "body : " << body << "\n";
}

/*response::response(const request::exception except, const parser &pars) : message(), first_line()
{
	first_line.status = except.get_status();
	first_line.version = HTTP_VERSION;

	request small_size_request(except);

	main_header();
	first_line.status = error_response(first_line.status, small_size_request, pars);
	first_line.status_string = find_status_string();
	first_line.version = HTTP_VERSION;
}

// 	main_header();
// 	first_line.status = error_response(first_line.status, small_size_request, pars);
// 	first_line.status_string = find_status_string();
// 	first_line.version = HTTP_VERSION;
// }*/

response::~response(){};
