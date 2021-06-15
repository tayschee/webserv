#include "cluster.hpp"

cluster::cluster() // constructor by defautl defined as private
{}

cluster::cluster(const std::string _path, bool debug_mode) : list_client(), debug_mode(debug_mode)// constructor used
{
	(void)_path;
    std::vector<parser::address_conf> vec_parser = parser::parse_folder(_path);
	size_t i = 0;
	size_t j = 0;

	std::cout << "START :\n";
	while (i < vec_parser.size())
	{
		j = 0;
		while (j < vec_parser[i].size())
		{
			std::cout << vec_parser[i][j].get_block(PARSER_SERVER).conf.find(PARSER_SERVER_NAME)->second << " ";
			++j;
		}
		std::cout << "\n";
		++i;
	}
	for (std::vector<parser::address_conf>::iterator it = vec_parser.begin(); it != vec_parser.end(); ++it)
	{
		int		sock = server((*it)[0]).get_socket_host();
		list_client.push_back(client(sock, true, &*it));
	}
}

cluster::cluster(const cluster& other) : debug_mode(other.debug_mode) // constructor by copy
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
	for(iterator it = list_client.begin(); it != list_client.end(); ++it)
		close(it->get_fd());
}