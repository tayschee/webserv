#include "parser.hpp"

std::vector<std::string> parser::split(const std::string &str, const std::string &delimiters)
{
	std::vector<std::string> result;

	size_t beg, pos = 0;
	while ((beg = str.find_first_not_of(delimiters, pos)) != std::string::npos)
	{
		pos = str.find_first_of(delimiters, beg + 1);
		result.push_back(str.substr(beg, pos - beg));
	}
	return result;
}

std::vector<parser *> parser::parse_folder(std::string path)
{
	std::vector<parser *> res;
	DIR *dir = opendir(path.c_str());

	if (path[path.length() - 1] == '/')
		path.erase(path.end());

	for (dirent *entry = readdir(dir); entry; entry = readdir(dir))
		if (entry->d_type == DT_REG)
			res.push_back(new parser(path + "/" + entry->d_name));
	closedir(dir);
	return res;
}

void parser::block::add_block(parser::block *b)
{
	blocks.push_back(b);
}
