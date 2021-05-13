#include "webserv.hpp"

int		main()
{
	//cluster cl("conf/conf2");
	//cl.init_listen();
	//cl.start();
	//std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;

	std::vector<parser> p = parser::parse_folder("conf/conf2");

	for (size_t i = 0; i < p.size(); i++)
		std::cout << p[i] << (i + 1 < p.size() ? "\n\n" : "");
	return 0;
}
