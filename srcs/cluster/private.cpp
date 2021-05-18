#include "cluster.hpp"

extern bool is_alive;

void sighandler(const int signal) // catch the signals
{
	if (signal)
		is_alive = 0;
}

void	cluster::close_client(iterator &it) // close a client
{
	std::cout << "Close client: " << it->get_fd() << std::endl;
	close(it->get_fd());
	iterator tmp = it;
	it--;
	list_client.erase(tmp);
	std::cout << "erase " << std::endl;	
}

void	cluster::set_list_fd(fd_set &readfds, fd_set &writefds, int &max) // initialize the list of sockets
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
	{
		if (!it->is_time())
		{
			if (it->get_fd() > max)
				max = it->get_fd();
			FD_SET(it->get_fd(), &readfds);
			FD_SET(it->get_fd(), &writefds);
		}
		else
			close_client(it);
	}
}

int	cluster::wait_activity(fd_set &readfds, fd_set &writefds) // wait for something to read
{
	int activity, max;
	struct timeval time_select;

	activity = 0;
	max = 0;
	while (!activity)
	{
		set_list_fd(readfds, writefds, max);
		signal(SIGINT, sighandler);
		if (!is_alive)
			return 0;
		time_select.tv_sec = 20;
		time_select.tv_usec = 0;
		std::cout << "WAIT..." << std::endl;
		if ((activity = select(max + 1, &readfds, NULL, NULL, &time_select)) < 0 && errno != EINTR)
			std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;
	}
	return 1;
}

int		cluster::receive(client &cli, const int &fd, iterator &it) // there is something to read
{
	int res, size;
	if (cli.is_listen()) // check if new client
	{
		sockaddr_in address_in;
		size = sizeof(address_in);
		if (!is_alive)
			return 0;
		res = accept(fd, (struct sockaddr *)&address_in, (socklen_t *)&size);
		if (res < 0)
			std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;
		list_client.push_back(client(res, false, cli));
	}
	else
	{
		cli.receive(); // no of new client / juste call receive
		std::cout << "recieve past" << std::endl;
		if (cli.is_read() == -1) // if method is void / close client
		{
			close_client(it);
			return -1;
		}
	}
	return 1;
}

int		cluster::send_response(client &cli) // send of response
{
		if(cli.sent() < 0)
		{
			std::cerr << "send()" << strerror(errno) << std::endl;
			return -1;
		}
		return 1;
}