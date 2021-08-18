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

/*this function return function associated to a method*/
response::method_array::mapped_type //method_function
response::find_method_function(const std::string &method, const std::vector<std::string> &allow_method) const
{
	std::vector<std::string>::const_iterator it(allow_method.begin());
	std::vector<std::string>::const_iterator end(allow_method.end());
	while (it < end)
	{
		if (method == *it)
			return existing_method.find(method)->second;
		++it;
	}
	return &response::method_is_unknow;
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

	// media_type_array::const_iterator	val(existing_media_type.find(subtype));

	// /*if subtype doesn't exist return default value*/
	// if (val == existing_media_type.end())
	// 	return (media_type_array::value_type(DEFAULT_SUBTYPE, DEFAULT_TYPE));

	// return (*val);
}

std::string	response::find_path(const parser::block &block, const std::string &partial_path, const request &req, const bool index) const
{
	(void)req;
	parser::entries entries(block.conf);
	std::string path;

	std::string alias;
	if (entries.find("alias") == entries.end())
		path = entries.find("root")->second[0] + partial_path;
	else
	{
		alias = entries.find("alias")->second[0];
		path = alias + std::string(partial_path.begin() + block.args[0].size(), partial_path.end());
	}

	struct stat file_stat;

	if (stat(path.c_str(), &file_stat) < 0)
	{
		//do something
	}
	else if (index && (file_stat.st_mode & S_IFMT) == S_IFDIR) //S_IFMT is a mask to find S_IFDIR which is value to directory
	{
		//determine if this is complete path or if this not for that verify if this is a directory
		if (is_open(file_stat))
			return path;
		std::list<std::string> files(files_in_dir(path));
		if (*(--path.end()) != '/')
			path.push_back('/');
		std::string ret = path + find_index(entries, files);
		return (ret);
	}
	else
	{
		return path;
		//return find_language(path, files_in_dir(path), req);
	}
	return path;
}

std::string response::find_index(const parser::entries &entries, const std::list<std::string> &files) const
{
	std::list<std::string>::const_iterator it_f;
	std::list<std::string>::const_iterator end_f;
	try
	{
		if (entries.find("index") == entries.end())
			return "";
		std::vector<std::string> index(entries.find("index")->second);
		std::vector<std::string>::iterator it_i(index.begin());
		std::vector<std::string>::iterator end_i(index.end());
		end_f = files.end();
		while (it_i != end_i)
		{
			it_f = files.begin();
			while (it_f != end_f)
			{
				if (*it_i == *it_f)
					return *it_i;
				++it_f;
			}
			++it_i;
		}
	}
	catch(const std::exception& e)
	{
		//std::cerr << e.what() << '\n';
	}
	return "";
}

const parser::address_conf::const_iterator	response::find_parser(const std::vector<parser::address_conf>::const_iterator &pars_list, const request &req) const
{
	size_t									i;
	parser::address_conf::const_iterator	end(pars_list->end());
	parser::address_conf::const_iterator	it(pars_list->begin());
	const std::string						host(req.get_header().find(HOST)->second);
	parser::address_conf::const_iterator	default_parser = pars_list->end();
	std::string 							port(it->get_block(PARSER_SERVER).conf.find(PARSER_LISTEN)->second[0]);

	while (it != end)
	{
		const parser::entries &map_server(it->get_block(PARSER_SERVER).conf);
		if (map_server.find("listen")->second.size() == 2)
			default_parser = it;
		const std::map<std::string, std::vector<std::string> >::const_iterator it_server_name(map_server.find(PARSER_SERVER_NAME));

		if (it_server_name == map_server.end())
			return default_parser;

		const std::vector<std::string> server_name_vec(map_server.find(PARSER_SERVER_NAME)->second);
		i = 0;
		while (i < server_name_vec.size())
		{
			std::cout << "hi\n";
			//std::cout << "|" << host << "| : |" << server_name_vec[i] << "|\n";
			if ((std::atoi(port.c_str()) == 80 && host == server_name_vec[i]) || host == (server_name_vec[i] + ":" + port))
			{

				//std::cout << it->get_block(PARSER_SERVER).conf.find(PARSER_LISTEN)->second[0] << " " 
				//<< it->get_block(PARSER_SERVER).conf.find(PARSER_LISTEN)->second[1] << "\n";
				//std::cout << server_name_vec[i] << "\n";
				return it;
			}
			++i;
		}
		++it;
	}
	
	if (pars_list->end() == default_parser)
	{
		return it;
	}
	return default_parser;
}
