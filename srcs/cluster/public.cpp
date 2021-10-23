#include "cluster.hpp"

int cluster::init_listen() // start sockets
{
	errno = 0;
	for (iterator it = list_client.begin(); it != list_client.end(); ++it)
	{
		if (listen((*it)->get_fd(), SOMAXCONN))
		{
			std::cerr << "Failed to listen. Error: " << strerror(errno) << std::endl;
			return 0;
		}
	}
	return (1);
}

int cluster::start() // cluster manage the list of socketc
{
	fd_set readfds, writefds;

	while (true)
	{
		if (!wait_activity(readfds, writefds))
			return 0;

		iterator end = list_client.end();
		for (iterator it = list_client.begin(); it != end; ++it)
		{
			client &cli = *(*it);

			if (FD_ISSET(cli.get_fd(), &readfds) && !receive(cli, writefds)) // is there a modification on the current list_client ?
			{
				close_client(it);
			}
			else if (cli.is_read() && !cli.sent(vec_parser, readfds, writefds))
			{
				close_client(it);
			}
		}
	}
	return (0);
}