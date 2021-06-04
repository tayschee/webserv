#include "webserv.hpp"
#include <iostream>

int		main()
{
	int a;
	int fd = open("file", O_RDONLY);
	message::receive rcv(fd, 27);

	while ((a = rcv()) != -1)
	{
		if (a == 1)
		{
			std::cout << "------------------------\n" << rcv.get() << "\n-----------------------\n";
			std::cout << "ok\n";
			while ((a = rcv.check()) == 1)
			{
				//std::cout << "check : " << rcv.check() << "\n";
				std::cout << "------------------------\n" << rcv.get() << "\n-----------------------\n";
			}
			//std::cout << "rcv_check() : " << a << "\n";
		}
	}

	close(fd);
	return 0;
}
