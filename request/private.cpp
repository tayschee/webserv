#include <request.hpp>

/* Store information about request (cmd, arg and version)*/
void			request::method_parsing(const std::string &commande_line)
{
	int i = 0;
	char **cmd_split;

	cmd_split = ft_split(commande_line.c_str(), ' '); //NEED FT_SPLIT PISCINE to verify tab + space

	while (cmd_split[i])
	{
		method[i] = cmd_split[i];
		++i;
	}
}

