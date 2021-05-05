/*  this file regroup all public functions from server.hpp
    which are not inside coplien.cpp and operator.cpp
*/

#include "server.hpp"
#include <list>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <stdio.h>
#include <string.h>
#include <iostream>

//bool is_alive = 1;
/*
std::list<client>::iterator		server::close_client(std::list<client>::iterator it)
{
	std::cerr << "Close client: " << it->get_fd() << std::endl;
	close(it->get_fd());
	std::list<client>::iterator tmp = it;
	it--;
	socket_client.erase(tmp);
	std::cerr << "erase " << std::endl;
	return it;
}

void sighandler(const int signal)
{
	if (signal)
		is_alive = 0;
}

void server::start()
{
	is_alive = 1;

	fd_set readfds, writefds;
	int activity, res, size, max;
	sockaddr_in address_in;
	std::list<client>::iterator it;
	struct timeval time_select;

	if (listen(socket_host, 12))
	{
		std::cerr << "Failed to listen. Error: " << strerror(errno) << std::endl;
		return ;
	}
	while (true)
	{
		int ret = 1;
		while (ret)
		{
			max = socket_host;
			signal(SIGINT, sighandler);
			if (!is_alive)
			{
				std::cout << "exit signal" << std::endl;
				server::~server();
				exit(0);
			}
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_SET(socket_host, &readfds);
			for (std::list<client>::iterator it = socket_client.begin(); it != socket_client.end() ; ++it)
			{
				if (!it->is_time())
				{
					if (it->get_fd() > max)
						max = it->get_fd();
					FD_SET(it->get_fd(), &writefds);
					FD_SET(it->get_fd(), &readfds);
				}
				else
					it = close_client(it);
			}
			max = std::max(std::max(socket_client.front().get_fd(), socket_client.back().get_fd()), socket_host);
			time_select.tv_sec = 5;
			time_select.tv_usec = 0;
			std::cout << "before select" << std::endl;
			if ((activity = select(max + 1, &readfds, NULL, NULL, &time_select)) < 0 && errno != EINTR)
				std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;
			if (activity)
				ret = 0;
		}

		if (FD_ISSET(socket_host, &readfds)) // If there is a modification on socket_host, then it's an incomming connection
		{
			size = sizeof(address_in);
			if (!is_alive)
				break;
			res = accept(socket_host, (struct sockaddr *)&address_in, (socklen_t *)&size);
			if (res < 0)
				std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;
			std::cerr << "New client: " << res << std::endl;
			socket_client.push_back(client(res));
		}

		for (it = socket_client.begin(); it != socket_client.end(); it++)
		{
			if (FD_ISSET(it->get_fd(), &readfds)) // is there a modification on the current socket_client ?
			{
				it->rq.receive(it->get_fd(), &it->rcm, 1024);
				it->set_time();
				it->is_read = true;
				std::cout << "client n" << it->get_fd() << std::endl;
				if (it->rq.get_method().empty()) // request empty... then close the connection
				{
					it = close_client(it);
					it->is_read = false;
				}
			}
			if (it->is_read && (activity = select(max + 1, NULL, &writefds, NULL, NULL)))
			{
				it->is_read = false;
				response rp(it->rq);
				if(send(it->get_fd(), rp.get().c_str() , rp.get().size(), 0) < 0)
				{
					std::cerr << "send()" << strerror(errno) << std::endl;
					exit(errno);
				}
	
				it->rq = request();
				it->rcm = message::receive_management();

				std::cout << "------------------------------" << std::endl;
				std::cout << "------------------------------" << std::endl;				

			//	it = close_client(it);
			}
			else if (it->is_time())
				it = close_client(it);
		}
	}
}*/
