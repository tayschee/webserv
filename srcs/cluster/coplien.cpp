#include "cluster.hpp"

cluster::cluster() // constructor by defautl defined as private
{}

cluster::cluster(const std::string _path) : list_client() // constructor used
{
    std::vector<parser> pars = parser::parse_folder(_path);
	for (std::vector<parser>::iterator it = pars.begin(); it != pars.end(); ++it)
	{
		int		sock = server(*it).get_socket_host();
		list_client.push_back(client(sock, true, *it));
	}
}

cluster::~cluster() // destructor
{
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
		close(it->get_fd());
}