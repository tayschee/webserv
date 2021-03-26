#include "request.hpp"

void request::print_cmd() const
{
	std::cout << "cmd : |" << cmd << "|" << std::endl;
}

void request::print_header() const
{
	std::map<std::string, std::string>::const_iterator it(header.begin());
	std::map<std::string, std::string>::const_iterator end(header.end());

	std::cout << "header : "<< std::endl;
	while (it != end)
	{
		std::cout << "key : |" << it->first << "|, value : |" << it->second << "|" << std::endl;
		++it;
	}
	std::cout << "end of header" << std::endl;
}

void request::print_body() const
{
	std::cout << "body : " << std::endl <<  "|" << body << "| end of body" << std::endl;
}

void request::print_variable() const
{
	print_cmd();
	std::cout << std::endl;
	print_header();
	std::cout << std::endl;
	print_body();
}