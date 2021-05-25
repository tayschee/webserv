#include "cluster.hpp"

bool is_alive;

int		cluster::init_listen() // start sockets
{
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
	{
		if(listen(it->get_fd(), 12))
		{
			std::cerr << "Failed to listen. Error: " << strerror(errno) << std::endl;
			return 0;
		}
	}
	return (1);
}

int 	cluster::start() // cluster manage the list of socket
{
	is_alive = 1;
	fd_set readfds, writefds;

	while (true)
	{
		if (!wait_activity(readfds, writefds))
			return 0;
		for(iterator it = list_client.begin(); it != list_client.end(); ++it)
		{
			int ret = 0;
			client cli = *it;
			if (FD_ISSET(it->get_fd(), &readfds)) // is there a modification on the current list_client ?
			{
				if((ret = receive(cli, it->get_fd(), it)) == 0)
					return 0;
			}
			if (ret > 0 && cli.is_read() == 1)
			{
				if (send_response(cli) == -1)
					close_client(it);
			}
			else if (cli.is_time())
				close_client(it);
		}
	}
	return (0);
}