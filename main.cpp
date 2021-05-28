#include "webserv.hpp"

int		main()
{
	std::vector<parser> pars = parser::parse_folder("./conf/conf2");

	//for (size_t i = 0; i < pars.size(); i++)
		//std::cout << pars[i] << std::endl;

	//cluster cl("./conf/conf2");
	//cl.init_listen();
	//cl.start();
	//std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;

	return 0;
}
