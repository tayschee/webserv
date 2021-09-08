#include "message/response.hpp"

response::response() : message()
{
}

response::response(const request &req, const parser::address_conf &pars_list) : message()
{
	first_line.status = 400;
	const parser::address_conf::const_iterator pars_it = find_parser(pars_list, req);
	std::cout << "ccccccccccccccccc" << std::endl;
	if (pars_it == pars_list.end())
	{
		std::cout << "PROBLEME DE PARSER" << std::endl;
		main_header();
		first_line.status = error_response(first_line.status, req);
	}
	else
	{
		const parser &pars(*pars_it);
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(PARSER_ACCEPT)->second); //no protect
		std::string path = find_path(pars.get_block(BLOCK_LOCATION, req.get_uri()), req.get_uri(), req);
		method_function method;

		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
		if (is_cgi(get_extension(path), pars, req.get_method()))
			method = existing_method.find(POST)->second; //give function associate with request
		else
			method = find_method_function(req.get_method(), allow_method); //give function associate with request

		main_header(); /*add header_field which are present in all method*/
		first_line.status = generate_response(path_info, pars, req, method);
	}
	first_line.status_string = find_status_string(first_line.status);
	first_line.version = req.get_version();
		// if (header.find(CONTENT_LENGTH) != header.end() && header.find(CONTENT_LENGTH)->second[0] == 0)
		std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	if (req.get_method() == "HEAD")
	{
		if (header.find(CONTENT_LENGTH) != header.end() && header.find(CONTENT_LENGTH)->second == "0")
			header.erase(CONTENT_LENGTH);
		if (first_line.status >= 300 && header.find(LAST_MODIFIED) != header.end())
		{
			header.erase(LAST_MODIFIED);
		}
		if (first_line.status >= 500)
			header.insert(value_type("Connection", "close"));
		else if (first_line.status >= 200)
			header.insert(value_type("Connection", "keep-alive"));
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

response::~response(){};