#include "webserv.hpp"

int		main()
{
	cluster cl("./conf/conf2");
	cl.init_listen();
	cl.start();
	std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	return 0;
}
