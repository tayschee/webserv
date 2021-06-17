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

std::vector<parser::address_conf> parser::parse_folder(std::string path)
{
	std::vector<address_conf> res;
	DIR *dir = opendir(path.c_str());

	mime = create_default_mime_type();

	if (path[path.length() - 1] == '/')
		path.erase(path.end());

	if (dir == NULL)
		throw std::invalid_argument("Cannot open " + path + ": no such file or directory.");

	for (dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
		if (entry->d_type == DT_REG && std::string(entry->d_name) == "mime")
			parse_mime(path + "/" + entry->d_name);
		else if (entry->d_type == DT_REG && (get_extension(entry->d_name) == ".conf"))
		{
			parser new_object(parser(path + "/" + entry->d_name));
			insert_parse_folder(res, new_object);
		}
	}

	names.clear();
	buffer.clear();

	closedir(dir);
	//if (res.empty())
		//throw std::runtime_error("All the files in " + path + " are invalid.");
	return res;
}

void		parser::insert_parse_folder(std::vector<address_conf> &pars, parser &new_object)
{
	size_t i = 0;

	if (!new_object.validate())
		return ;

	entries pars_new_object(new_object.get_block(PARSER_SERVER).conf);

	while (i < pars.size())
	{
		entries pars_block(pars[i][0].get_block(PARSER_SERVER).conf); //just check host and port of first block to know if he is in same categorie

		if (pars_block.find(PARSER_HOST)->second == pars_new_object.find(PARSER_HOST)->second
			&& pars_block.find(PARSER_LISTEN)->second == pars_new_object.find(PARSER_LISTEN)->second)
		{
			pars[i].push_back(new_object);
			return ;
		}
		++i;
	}
	pars.push_back(std::vector<parser>(1, new_object));
}

const parser::block &parser::get_block(const std::string& block_name, const std::vector<std::string>& block_args) const
{
	std::cout << "ok2\n";
	blocks::key_type key = std::make_pair(block_name, block_args);
	blocks::const_iterator it = _blocks.find(key);
	std::cout << "ok3\n";

	std::cout << "ok4\n";
	if (block_name == "location" && it == _blocks.end())
	{
		std::cout << "ok4.5\n";
		return get_block(block_name, find_best_match(block_args.empty() ? "" : block_args[0]));
	}
	else if (it == _blocks.end())
	{
		std::cout << "ok5\n";
		throw BlockNotFound(block_name, block_args);
	}
	std::cout << it->second << "ok6\n";
	return it->second;
}

const parser::block &parser::get_block(const std::string& block_name, const std::string& block_arg) const
{
	std::vector<std::string> args;

	std::cout << "ok\n";
	args.push_back(block_arg);
	std::cout << "ok1\n";
	return get_block(block_name, args);
}

const char *parser::BlockNotFound::what() const throw()
{
	return err.c_str();
}
