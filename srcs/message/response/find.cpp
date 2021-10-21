#include "message/response.hpp"
#include "message/request.hpp"

/*this file regroup get function, get dont refer to the method but function to get a value through an other*/

/*return string associated to a status*/
response::status_array::value_type::second_type //std::string
response::find_status_string(const int status) const
{
	status_array::const_iterator it(existing_status.find(status));

	if (it == existing_status.end())
		return UNKNOW_STATUS;
	else
	{
		return existing_status.find(status)->second;
	}
}

std::string		response::find_method_allow(const request &req, const parser &pars, const std::string &method)
{
	std::vector<std::string> allow_method(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf.find(PARSER_ACCEPT)->second); //no protect
	std::vector<std::string>::const_iterator it(allow_method.begin());
	std::vector<std::string>::const_iterator end(allow_method.end());
	while (it < end)
	{
		if (method == *it)
			return method;
		++it;
	}
	return "unknow";
}

/*this function return function associated to a method*/
response::method_array::mapped_type //method_function
response::find_method_function(const request &req, const std::string &method, const parser &pars) const
{
	if (method == "unknow")
		return &response::method_is_unknow;
	std::string path = find_path(pars, req.get_uri(), req);
	if (is_cgi(get_extension(path), pars, method))
		return existing_method.find(POST)->second;
	return existing_method.find(method)->second;
}

//response::media_type_array::value_type //std::pair<std::string, std::string>
std::string
response::find_media_type(const std::string subtype, const parser &pars) const
{
	std::string type;
	if (subtype.empty())
		return subtype;
	try
	{
		parser::entries block = pars.get_mime().conf;

		if (block.find(subtype) != block.end())
		{
			type = block.find(subtype)->second[0];
		}
		else
			type = "";
	}
	catch(const std::exception& e)
	{
		type = "";
	}
	return type;
}

std::string	response::find_path(const parser &pars, const std::string &partial_path, const request &req, const bool index) const
{
	(void)req;
	parser::block block(pars.get_block(BLOCK_LOCATION, partial_path));
	std::string path;

	std::string alias;

	if (block.conf.find(ALIAS) == block.conf.end())
	{
		path = block.conf.find(BLOCK_ROOT)->second[0] + partial_path;
	}
	else
	{
		alias = block.conf.find(ALIAS)->second[0];
		std::string root;
		if (pars.get_block(BLOCK_LOCATION, alias).conf.find(BLOCK_ROOT) != block.conf.end())
			root =  pars.get_block(BLOCK_LOCATION, alias).conf.find(BLOCK_ROOT)->second[0];
		else
			root = pars.get_block(BLOCK_SERVER).conf.find(BLOCK_ROOT)->second[0];
		path = root + std::string(partial_path.begin() + block.args[0].size(), partial_path.end());
	}

	struct stat file_stat;

	size_t pos = path.find("//");
	if (pos != path.npos)
		path.erase(pos, 1);
	if (stat(path.c_str(), &file_stat) < 0)
	{
	}
	else if (index && (file_stat.st_mode & S_IFMT) == S_IFDIR) //S_IFMT is a mask to find S_IFDIR which is value to directory
	{
		//determine if this is complete path or if this not for that verify if this is a directory
		if (is_acces(file_stat))
			return path;
		if (*(--path.end()) != '/')
			path.push_back('/');
		return find_index(block.conf, path);
	}
	else
	{
		return path;
	}
	return path;
}

std::string response::find_index(const parser::entries &entries, const std::string &path) const
{
	try
	{
		if (entries.find("index") == entries.end())
			return path;
		std::vector<std::string> index(entries.find("index")->second);
		std::vector<std::string>::iterator it_i(index.begin());
		std::vector<std::string>::iterator end_i(index.end());

		if (entries.find("index") == entries.end())
			return path;
		std::string tmp;
		struct stat file_stat;

		while (it_i != end_i)
		{
			tmp = path + *it_i;
			if (stat(tmp.c_str(), &file_stat) >= 0)
				return tmp;
			it_i++;
		}
	}
	catch(const std::exception& e)
	{
	}

	return path;
}

const parser::address_conf::const_iterator	response::find_parser(const parser::address_conf &pars_list	, const request &req) const
{
	parser::address_conf::const_iterator it;
	parser::address_conf::const_iterator it_default = pars_list.begin();
	for (it = pars_list.begin(); it != pars_list.end(); ++it)
	{
		if (it->get_block(BLOCK_SERVER).conf.find(LISTEN)->second.size() == 2 && it->get_block(BLOCK_SERVER).conf.find(LISTEN)->second[1] == DEFAULT_SERVER)
			it_default = it;
		if (it->get_block(BLOCK_SERVER).conf.find(SERVER_NAME) != it->get_block(BLOCK_SERVER).conf.end())
		{
			if (it->get_block(BLOCK_SERVER).conf.find(SERVER_NAME)->second[0] == req.get_host().substr(0, req.get_host().find(":")))
				break;
		}
	}
	if (it == pars_list.end())
		it = it_default;
	return it;
}