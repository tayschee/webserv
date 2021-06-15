#include "webserv.hpp"
#include <iostream>

int		main()
{
	cluster cl("./conf/conf2", true);
	cl.init_listen();
	cl.start();
	std::cout << "WEBSERV C'EST FERMER CORRECTEMENT" << std::endl;
	
	return 0;
}
