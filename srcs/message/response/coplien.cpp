#include "message/response.hpp"
#include <iostream>

void	response::get_code(const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	status_header();
	std::string file_error = "/home/user42/42/webserv/error/" + std::string(ft_itoa(first_line.status)) + ".html";
	if (lstat(file_error.c_str(), &file_stat) < 0)
	{
		file_error = "/home/user42/42/webserv/error/404.html";
		lstat(file_error.c_str(), &file_stat);
	}
	int fd = open(file_error.c_str(), O_RDONLY);
	char buf[file_stat.st_size + 1];
	int ret = read(fd, buf, file_stat.st_size);
	buf[ret] = '\0';
	body = buf;
	header.insert(value_type(CONTENT_LENGTH,  ft_itoa(body.size())));
	header.insert(value_type(CONTENT_TYPE,  "text/html"));
	header.insert(value_type("Request Method",  GET));
	close(fd);
}

bool		response::is_redirect(parser::entries &block, const parser &pars)
{
//	try
//	{
		(void)pars;
		std::string redirect;

		if (block.find("return") == block.end())
			return 0;

		redirect = block.find("return")->second[0];

		if (!redirect.empty())
		{

			first_line.status = ft_atoi<int>(redirect);
		//	header.insert(value_type(CONTENT_TYPE, "application/octet-stream"));


			std::string location = block.find("return")->second[1];

			if (first_line.status == 301 || first_line.status == 302 || first_line.status == 303
			|| first_line.status == 307 || first_line.status == 308)
			{
				header.insert(value_type(LOCATION, location));
				//get_code(pars);
			}
			else
				body = location;
			return 1;
		}
//	}
//	catch(const std::exception& e)
//	{
//	}
	return 0;
}

response::response(const request &req, const parser &pars) : message(), first_line()
{
	std::cout << "allo?\n";
	if (req.validity(pars) != 0)
	{
		first_line.status = 400;
		//get_code(pars);
		first_line.status_string = find_status_string();
	}
	else
	{
		std::cout << "ok\n";
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::cout << "ok1\n";
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

