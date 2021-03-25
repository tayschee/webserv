#include "request.hpp"

void request::print_cmd() const
{
	std::cout << "cmd : |" << cmd << "|" << std::endl;
}

void request::print_header_data() const 
{
	std::map<std::string, std::string>::const_iterator it(header_data.begin());
	std::map<std::string, std::string>::const_iterator end(header_data.end());

	std::cout << "header_data : "<< std::endl;
	while (it != end)
	{
		std::cout << "key : |" << it->first << "|, value : |" << it->second << "|" << std::endl;
		++it;
	}
	std::cout << "end of header_data" << std::endl;
}

void request::print_message() const
{
	std::cout << "message : " << std::endl <<  "|" << message << "| end of message" << std::endl;
}

void request::print_variable() const
{
	print_cmd();
	std::cout << std::endl;
	print_header_data();
	std::cout << std::endl;
	print_message();
}