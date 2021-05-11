#include "cluster.hpp"

int		main()
{
	cluster cl("/home/user42/42/webserv/conf/conf2");
	cl.init_listen();
	try
	{
		cl.start();
	}
	catch (std::string const e)
	{
		std::cout << "GGGGGGGGGGGGGggggggggggg" << std::endl;
	}
	std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	return 0;
}