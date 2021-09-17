#include "cluster.hpp"

cluster::cluster() // constructor by defautl defined as private
{}

cluster::cluster(const std::string _path) : list_client()// constructor used
{
    vec_parser = parser::parse_folder(_path);

	int i = 0;
	for (std::vector<parser::address_conf>::iterator it = vec_parser.begin(); it != vec_parser.end(); ++it)
	{
		int		sock = server((*it)[0]).get_socket_host();
		list_client.push_back(new client(sock, true, i));
		++i;
	}
}

cluster::cluster(const cluster& other) // constructor by copy
{
	(void)other;
}

cluster &cluster::operator=(const cluster& other) // assignation
{
	(void)other;
	return *this;
}

cluster::~cluster() // destructor
{
	iterator tmp;
	iterator it = list_client.begin();
	while (it != list_client.end())
	{
		tmp = it;
		it++;
		close((*tmp)->get_fd());
		delete (*tmp);
	}
}