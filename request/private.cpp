#include <request.hpp>

/* Store information about request (cmd, arg and version)*/
void			request::method_parsing(const std::string &commande_line)
{
	size_t i = 0;
	std::vector<std::string> cmd_split;

	cmd_split = split(commande_line.c_str(), " "); //NEED FT_SPLIT PISCINE to verify tab + space

	while (i < cmd_split.size())
	{
		method[i] = cmd_split[i];
		++i;
	}
}

