#include "webserv.hpp"
#include <iostream>

// void	tt()
// {
// 	message::receive r;
// 	r.~receive();
// }

int		main()
{
	/*std::vector<parser> pars = parser::parse_folder("./conf/conf2");

	//parser::block block = pars[1].get_block(PARSER_SERVER);
	parser::block block = pars[0].get_block(PARSER_LOCATION, "/");
	parser::entries map = block.conf;

	parser::entries::iterator it = map.begin();
	parser::entries::iterator end = map.end();


	while (it != end)
	{
		std::cout << it->first << " : "  << it->second << "\n";
		++it;
	}*/
	// int i = 0;
	// while (i++ < 20)
	// {
	// 	tt();
	// 	usleep(2000000);
	// 	std::cout << "===============" << std::endl;
	// }

	cluster cl("./conf/conf4", true);
	cl.init_listen();
	try
	{
		cl.start();
		std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;
	}	
	catch(const std::string& e)
	{
		std::cout << "WEBSER C'EST FERMER AVEC CATCH" << std::endl;
	}

	// char *a = NULL;

	// delete a;
	
	return 0;
}
