#include "cluster.hpp"

bool is_alive;

int		cluster::init_listen() // start sockets
{
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
	{
		if(listen((*it)->get_fd(), SOMAXCONN))
		{
			std::cerr << "Failed to listen. Error: " << strerror(errno) << std::endl;
			return 0;
		}
		else
		{
			if (debug_mode)
				std::cout << "Connection etablished with " << (*it)->get_fd() << std::endl; //change to put ip + port will be better
		}
	}
	return (1);
}

int 	cluster::start() // cluster manage the list of socketc
{
	is_alive = 1;
	fd_set readfds, writefds;

	while (true)
	{
		if (!wait_activity(readfds, writefds))
			return 0;
	
		iterator end = list_client.end();
		for(iterator it = list_client.begin(); it != end; ++it)
		{
			int ret = 0;
			client &cli = *(*it);
			
			if (!cli.is_read() && FD_ISSET(cli.get_fd(), &readfds)) // is there a modification on the current list_client ?
			{
				// if (debug_mode)
				// 	std::cout << "Receive message from " << cli.get_fd() << std::endl;
				
				if ((ret = receive(cli)) == 0)
					return 0;
				else if (ret == -1)
				{
					if (debug_mode)
						std::cout << "Client quit : ";
					close_client(it);
				}
		}
			else if (cli.is_read() && FD_ISSET(cli.get_fd(), &writefds))
			{
				cli.sent(vec_parser);
			}
		}
	}
	return (0);
}