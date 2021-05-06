#include "../include/cluster.hpp"

bool is_alive;
void sighandler(const int signal)
{
	if (signal)
		is_alive = 0;
}

cluster::cluster(const std::string _path) : list_client(), tab(), pars()
{
    pars = parser::parse_folder(_path);
	for (std::vector<parser>::iterator it = pars.begin(); it != pars.end(); ++it)
	{
		tab.push_back(server(*it));
		int		sock = tab.back().get_socket_host();
		list_client.push_back(client(sock, true, *it));
	}
}

int		cluster::init_listen()
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

int 	cluster::start()
{
	is_alive = 1;
	fd_set readfds, writefds;
	int activity, max, res, size;
	sockaddr_in address_in;
	struct timeval time_select;

	while (true)
	{
		activity = 0;
		max = tab[0].get_socket_host();
		while (!activity)
		{
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			for(iterator it = list_client.begin(); it != list_client.end(); ++it)
			{
				if (!it->is_time())
				{
					if (it->get_fd() > max)
						max = it->get_fd();
					FD_SET(it->get_fd(), &writefds);
					FD_SET(it->get_fd(), &readfds);
				}
				else
					close_client(it);
			}
			signal(SIGINT, sighandler);
			if (!is_alive)
				return 0;
			time_select.tv_sec = 20;
			time_select.tv_usec = 0;
			if ((activity = select(max + 1, &readfds, NULL, NULL, &time_select)) < 0 && errno != EINTR)
				std::cerr << "Failed to select. Error: " << strerror(errno) << std::endl;
		}

		for(iterator it = list_client.begin(); it != list_client.end(); ++it)
		{
			int fd = it->get_fd();
			client cl = *it;
			if (FD_ISSET(fd, &readfds)) // is there a modification on the current list_client ?
			{
				if (cl.is_listen())
				{
					size = sizeof(address_in);
					if (!is_alive)
						break;
					res = accept(fd, (struct sockaddr *)&address_in, (socklen_t *)&size);
					if (res < 0)
						std::cerr << "Failed to accept. Error: " << strerror(errno) << std::endl;
					list_client.push_back(client(res, false, cl.get_pars()));
				}
				else
				{
					cl.is_read = cl.rq.receive(fd, cl.rcm);

					cl.set_time();
					if (cl.rq.get_method().empty()) // request empty... then close the connection
						close_client(it);
				}
			}
			if (cl.is_read && (activity = select(max + 1, NULL, &writefds, NULL, NULL)))
			{
				cl.is_read = false;
				response rp(cl.rq, cl.get_pars());
				//std::cout << rp.get() << std::endl;
				if(send(fd, rp.get().c_str() ,rp.get().size(), 0) < 0)
				{
					std::cerr << "send()" << strerror(errno) << std::endl;
					exit(errno);
				}
			
				cl.rq = request();
				cl.rcm = message::receive_management();
			}
			else if (cl.is_time())
				close_client(it);
		}
	}
	return (0);
}

void	cluster::close_client(iterator &it)
{
	std::cout << "Close client: " << it->get_fd() << std::endl;
	close(it->get_fd());
	iterator tmp = it;
	it--;
	list_client.erase(tmp);
	std::cout << "erase " << std::endl;	
}

cluster::cluster()
{}

cluster::~cluster()
{
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
		close(it->get_fd());
}