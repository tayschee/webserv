/*  this file regroup all public functions from server.hpp
    which are not inside coplien.cpp and operator.cpp
*/

#include "server.hpp"

void server::start()
{
	fd_set readfds;
	int activity, res, size, max;
	sockaddr_in address_in;
	std::vector<int>::iterator it;

	char buffer[1025];

	if (listen(socket_host, 12))
	{
		std::cerr << "Failed to listen. Error: " << strerror(errno) << std::endl;
		return ;
	}

	while (true)
	{
		FD_ZERO(&readfds);
		FD_SET(socket_host, &readfds);

		for (size_t i = 0; i < socket_client.size(); i++)
			if (socket_client[i] > 0)
				FD_SET(socket_client[i], &readfds);
		max = std::max(*std::max_element(socket_client.begin(), socket_client.end()), socket_host);

		// returns when select detects a ready fd.
		activity = select(max + 1, &readfds, NULL, NULL, NULL);

		if (activity < 0 && errno != EINTR)
			std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;

		if (FD_ISSET(socket_host, &readfds)) // If there is a modification on socket_host, then it's an incomming connection
		{
			res = accept(socket_host, (struct sockaddr *)&address_in, (socklen_t *)&size);
			if (res < 0)
				std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;

			// finding the first 0 of socket_client and setting it
			it = std::find(socket_client.begin(), socket_client.end(), 0);
			if (it != socket_client.end())
				*it = res;
		}

		for (size_t i = 0; i < socket_client.size(); i++)
		{
			if (FD_ISSET(socket_client[i], &readfds)) // is there a modification on the current socket_client ?
			{
				res = read(socket_client[i], buffer, 1024); // TODO : make this read the whole message

				if (res > 0)
				{
					buffer[res] = 0;
					std::cout << buffer;
				}
				else if (!res) // request empty... then close the connection
				{
					close(socket_client[i]);
					socket_client[i] = 0;
				}
				else
					std::cerr << "read got a problem. Error: " << strerror(errno) << std::endl;
			}
		}
	}
}
