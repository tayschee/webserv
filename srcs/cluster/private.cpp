#include "cluster.hpp"

extern bool is_alive;

void	cluster::close_client(iterator &it) // close a client
{
	iterator tmp = it;
	--it;
	delete(*tmp);
	list_client.erase(tmp);
}

void	init_fd(int fd, fd_set &readfds, fd_set &writefds, int &max)
{
	if (fd > 0)
	{
		if (fd > max)
			max = fd;
		FD_SET(fd, &readfds);
		FD_SET(fd, &writefds);
	}
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
			init_fd((*it)->get_fd(), readfds, writefds, max);
			init_fd((*it)->get_fdbody(), readfds, writefds, max);
			init_fd((*it)->get_fdout(), readfds, writefds, max);
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
		signal(SIGPIPE, SIG_IGN);
		if (!is_alive)
			return 0;
		time_select.tv_sec = 20;
		time_select.tv_usec = 0;

		if ((activity = select(max + 1, &readfds, &writefds, NULL, &time_select)) < 0 && errno != EINTR)
			std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;
	}
	return 1;
}

int		cluster::receive(client &cli, const fd_set &writefds) // there is something to read
{
	int res;//, size;
	if (cli.is_listen()) // check if new client
	{
		if (!is_alive)
			return 0;
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		res = accept(cli.get_fd(), (sockaddr*)&addr, &len);
		errno = 0;
		if (res < 0)
			std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;
		else
		{
			try
			{
				list_client.push_back(new client(res, false, cli.get_nb_pars()));
			}
			catch(std::exception &e)
			{
				(void)e;
			}
		}
	}
	else
		return cli.receive(writefds);
	return 1;
}