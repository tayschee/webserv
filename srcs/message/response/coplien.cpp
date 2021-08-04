#include "message/response.hpp"
#include <iostream>

response::response() : message()
{
}

response::response(const request &req, const parser &pars) : message()
{
	// std::cout << "allo?\n";
	// if (req.validity(pars) != 0)
	// {
	// 	first_line.status = 400;
	// 	//get_code(pars);
	// 	//first_line.status_string = find_status_string();
	// }
	// else
	// {
	pars.get_block(BLOCK_SERVER);
	parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
	std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
	std::string path = find_path(pars.get_block(BLOCK_LOCATION , req.get_uri()), req.get_uri(), req);
	method_function header_field_function;
	if (is_cgi(get_extension(path), pars, req.get_method()))
		header_field_function = existing_method.find(POST)->second; //give function associate with request
	else
		header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request
	main_header(allow_method);														  /*add header_field which are present in all method*/
	/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
	if (path_info.find(BODY_SIZE) != path_info.end() && req.get_body().size() > ft_atoi<unsigned long>(path_info.find("body_size_max")->second[0]))
	{
		first_line.status = 413;
		get_code(pars);
	}
	else if (!is_redirect(path_info, pars))
	{
		first_line.status = (this->*header_field_function)(req.get_uri(), req, pars);
		if (first_line.status > 299)
			get_code(pars);
	}
	//	}

	first_line.status_string = find_status_string();
	first_line.version = req.get_version();
	first_line.version = "HTTP/1.1";
}

// response::response(const request::exception except, const parser &pars) : message(), first_line()
// {
// 	first_line.status = except.get_status();
// 	first_line.version = HTTP_VERSION;

// 	request small_size_request(except);

// 	main_header();
// 	first_line.status = error_response(first_line.status, small_size_request, pars);
// 	first_line.status_string = find_status_string();
// 	first_line.version = HTTP_VERSION;
// }

response::~response(){};
