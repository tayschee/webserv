#include "parser.hpp"
#include "utils.hpp"

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

std::vector<parser> parser::parse_folder(std::string path)
{
	std::vector<parser> res;
	DIR *dir = opendir(path.c_str());

	if (path[path.length() - 1] == '/')
		path.erase(path.end());

	if (dir == NULL)
		throw std::invalid_argument("Cannot open " + path + ": no such file or directory.");

	for (dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
		if (entry->d_type == DT_REG && (get_extension(entry->d_name) == ".conf"))
		{
			res.push_back(parser(path + "/" + entry->d_name));
			if (!res.rbegin()->is_valid())
				res.pop_back();
		}
	}
	closedir(dir);
	if (res.empty())
		throw std::runtime_error("All the files in " + path + " are invalid.");
	return res;
}

const parser::block &parser::get_block(const std::string& block_name, const std::vector<std::string>& block_args) const
{
	blocks::key_type key = std::make_pair(block_name, block_args);
	blocks::const_iterator it = _blocks.find(key);

	if (block_name == "location" && it == _blocks.end())
		return get_block(block_name, find_best_match(block_args.empty() ? "" : block_args[0]));
	else if (it == _blocks.end())
		throw BlockNotFound(block_name, block_args);
	return it->second;
}

const parser::block &parser::get_block(const std::string& block_name, const std::string& block_arg) const
{
	std::vector<std::string> args;

	args.push_back(block_arg);
	return get_block(block_name, args);
}

const char *parser::BlockNotFound::what() const throw()
{
	return err.c_str();
}
