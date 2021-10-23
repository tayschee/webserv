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

std::vector<std::string> parser::split2(const std::string &str, const std::string &delimiters)
{
	std::vector<std::string> result;

	size_t beg, pos = 0;

	while ((beg = str.find_first_not_of(delimiters, pos)) != std::string::npos)
	{
		pos = beg;
		while ((pos = str.find_first_of(delimiters, pos)) && pos != std::string::npos && (std::count(str.begin(), str.begin() + pos, '\"') % 2) != 0)
			pos++;
		result.push_back(str.substr(beg, pos - beg));
	}
	return result;
}

std::vector<parser::address_conf> parser::parse_folder(std::string path)
{
	std::vector<address_conf> res;
	DIR *dir = opendir(path.c_str());

	mime = create_default_mime_type();

	if (dir == NULL)
		throw std::invalid_argument("Cannot open " + path + ": no such file or directory.");

	for (dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
		if (entry->d_type == DT_REG && std::string(entry->d_name) == "mime")
		{
			parse_mime(path + "/" + entry->d_name);
		}
		else if (entry->d_type == DT_REG && (get_extension(entry->d_name) == ".conf"))
		{
			parser new_object(path + "/" + entry->d_name);
			if (!insert_parse_folder(res, new_object))
				throw std::runtime_error("The file " + path + "/" + entry->d_name + " is invalid.");
		}
	}

	names.clear();
	buffer.clear();

	closedir(dir);
	chk_def_server(res);
	if (res.empty())
		throw std::runtime_error("All the files in " + path + " are invalid.");
	return res;
}

void parser::chk_def_server(const std::vector<address_conf> &pars)
{
	typedef std::vector<address_conf>::const_iterator const_iterator;

	for (const_iterator it = pars.begin(); it != pars.end(); it++)
	{
		if (it->empty())
			continue;

		bool default_server = false;
		parser first = (*it)[0];
		std::string host = first.get_block(PARSER_SERVER).conf.at(PARSER_HOST)[0];
		std::string port = first.get_block(PARSER_SERVER).conf.at(PARSER_LISTEN)[0];
		std::string host_port = host + ":" + port;

		for (address_conf::const_iterator it1 = it->begin(); it1 != it->end(); it1++)
		{
			if (it1->get_block(PARSER_SERVER).conf.at(PARSER_LISTEN).size() > 1)
			{
				if (!default_server)
					default_server = true;
				else
					throw std::runtime_error("There is more than one default_server for " + host_port);
			}
		}
	}
}

bool parser::insert_parse_folder(std::vector<address_conf> &pars, parser &new_object)
{
	size_t i = 0;

	if (!new_object.validate())
		return false;


	entries pars_new_object(new_object.get_block(PARSER_SERVER).conf);

	while (i < pars.size())
	{
		entries pars_block(pars[i][0].get_block(PARSER_SERVER).conf); //just check host and port of first block to know if he is in same categorie

		if (pars_block.find(PARSER_HOST)->second == pars_new_object.find(PARSER_HOST)->second && pars_block.find(PARSER_LISTEN)->second[0] == pars_new_object.find(PARSER_LISTEN)->second[0])
		{
			pars[i].push_back(new_object);
			return (true);
		}
		++i;
	}
	pars.push_back(std::vector<parser>(1, new_object));
	return true;
}

const parser::block &parser::get_block(const std::string &block_name, const std::vector<std::string> &block_args) const
{
	blocks::key_type key = std::make_pair(block_name, block_args);
	blocks::const_iterator it = _blocks.find(key);

	if (block_name == "location" && it == _blocks.end())
	{
		return get_block(block_name, find_best_match(block_args.empty() ? "" : block_args[0]));
	}
	else if (it == _blocks.end())
	{
		throw BlockNotFound(block_name, block_args);
	}
	return it->second;
}

const parser::block &parser::get_block(const std::string &block_name, const std::string &block_arg) const
{
	std::vector<std::string> args;

	args.push_back(block_arg);
	return get_block(block_name, args);
}

const parser::block &parser::get_mime()
{
	return mime;
}

const char *parser::BlockNotFound::what() const throw()
{
	return err.c_str();
}
