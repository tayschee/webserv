#include "cluster.hpp"
#include "message.hpp"

int		main()
{
	cluster cl("/home/user42/42/webserv/conf/conf2");
	cl.init_listen();
	try
	{
		cl.start();
		std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	}
	catch (std::string const e)
	{
	}
	return 0;
}
