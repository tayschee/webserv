#include <iostream>
#include "parser.hpp"

int main()
{
	std::vector<parser> parsed = parser::parse_folder("conf");

	parser p = parsed[1];

	std::cout << "\033[31;1mTest of get_block :\033[0m" << std::endl;
	std::cout << p.get_block("cgi");


	std::cout << "\n\033[31;1mTest of operator[] :\033[0m" << std::endl;
	parser::entries entries = p["test"];

	for (parser::entries::iterator it = entries.begin(); it != entries.end(); it++)
		std::cout << " - " << it->first << " -> " << it->second << std::endl;
	return (0);
}
