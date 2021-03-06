#include "cluster.hpp"

extern bool is_alive;

void sighandler(const int signal) // catch the signals
{
	if (signal)
		is_alive = 0;
}

void	cluster::close_client(iterator &it) // close a client
{
	if (debug_mode)
		std::cout << "Client closed : " << (*it)->get_fd() << std::endl;
	iterator tmp = it;
	--it;
	delete(*tmp);
	list_client.erase(tmp);
}

void	cluster::set_list_fd(fd_set &readfds, fd_set &writefds, int &max) // initialize the list of sockets
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
	{
		if ((*it)->is_reset())
		{
			(*it)->reset_time();
		}
		if (!(*it)->is_time())
		{
			if ((*it)->get_fd() > max)
				max = (*it)->get_fd();
			FD_SET((*it)->get_fd(), &readfds);
			FD_SET((*it)->get_fd(), &writefds);
		}
		else
		{
			close_client(it);
		}
	}
}

int	cluster::wait_activity(fd_set &readfds, fd_set &writefds) // wait for something to read
{
	int activity, max;
	activity = 0;
	max = 0;
	struct timeval time_select;

	while (!activity)
	{
		errno = 0;
		set_list_fd(readfds, writefds, max);
		signal(SIGINT, sighandler);
		if (!is_alive)
			return 0;
		time_select.tv_sec = 20;
		time_select.tv_usec = 0;
		if ((activity = select(max + 1, &readfds, &writefds, NULL, &time_select)) < 0 && errno != EINTR)
		{
			std::cout << "ERROR" << std::endl;
			std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;
		}
	}
	return 1;
}

int		cluster::receive(client &cli) // there is something to read
{
	int res;//, size;
	if (cli.is_listen()) // check if new client
	{
		if (!is_alive)
			return 0;
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		res = accept(cli.get_fd(), (sockaddr*)&addr, &len);
		if (res < 0)
			std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;
		else //change that
		{
			if (debug_mode)
				std::cout << "Connection etablished with " << cli.get_fd() << " -> " << res << "\n";
			list_client.push_back(new client(res, false, cli.get_nb_pars()));
		}
	}
	else
	{
		cli.receive();
		if (cli.is_read() == -1)
			return -1;
	}
	return 1;
}