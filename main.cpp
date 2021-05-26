#include "webserv.hpp"

int		main()
{
	cluster cl("conf/conf2");
	try
	{
		cl.init_listen();
		cl.start();
		std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	}
	catch(const std::string &e)
	{
	}
	
	return 0;
}
