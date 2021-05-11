#include "cluster.hpp"

int		main()
{
	cluster cl("/home/user42/42/webserv/conf/conf2");
	cl.init_listen();
	cl.start();
	std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	return 0;
}