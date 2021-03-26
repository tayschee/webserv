#include "request.hpp"

void request::print_method() const
{
	std::cout << "CMD : " << method[CMD] << "\n";
	std::cout << "ARG : " << method[ARG] << "\n";
	std::cout << "VERSION : " << method[VERSION] << "\n";
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
	print_method();
	std::cout << std::endl;
	print_header();
	std::cout << std::endl;
	print_body();
}