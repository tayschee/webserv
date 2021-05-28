#include "webserv.hpp"

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

	cluster cl("./conf/conf2");
	cl.init_listen();
	cl.start();
	std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;

	return 0;
}
