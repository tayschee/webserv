#include "webserv.hpp"
#include <iostream>

int		main()
{
	/*int a;
	int fd = open("file", O_RDONLY);
	message::receive rcv(fd, 27);

	while ((a = rcv()) != -1)
	{
		//std::cout << a << "\n";
		if (a == rcv.HEADER_END)
		{
			std::cout << "//////////////////////////////\n" << rcv.get_header_buffer() << "\n-----------------------\n";
		}
		else if (a == rcv.BODY_END)
		{
			std::cout << "------------------------\n" << rcv.get_buffer() << "\n-----------------------\n";
			rcv.prepare_next();
			while ((a = rcv.check()) == 1)
			{
				//std::cout << "check : " << rcv.check() << "\n";
				std::cout << "------------------------\n" << rcv.get_buffer() << "\n-----------------------\n";
				rcv.prepare_next();
			}
			//std::cout << "rcv_check() : " << a << "\n";
		}
	}*/
	//std::cout << a << "\n";
	//std::cout << "------------------------\n" << rcv.get_buffer() << "\n-----------------------\n";
	//close(fd);

	cluster cl("conf/conf2", true);
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
