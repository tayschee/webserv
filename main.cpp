#include "webserv.hpp"
#include <iostream>

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

	/*cluster cl("./conf/conf2", true);
	cl.init_listen();
	cl.start();
	std::cout << "WEBSER C'EST FERMER CORRECTEMENT" << std::endl;*/

	int fd = open("file", O_RDONLY);
	message::receive rcv(fd, 1);
	int i;
	std::string msg;

	while (1)
	{
		i = rcv();
		if (i == -1)
		{
			return 1;
		}
		else
		{
			while ((rcv.check() & rcv.BODY_MASK))
			{
				msg = rcv.get_buffer();
				rcv.prepare_next();
				std::cout << "------------------\n" << msg << "------------------\n";
			}
		}
	}
	
	return 0;
}
