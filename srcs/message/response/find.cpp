#include "message/response.hpp"

/*this file regroup get function, get dont refer to the method but function to get a value through an other*/

/*return string associated to a status*/
response::status_array::value_type::second_type //std::string
response::find_status_string() const
{
	status_array::const_iterator it(existing_status.find(first_line.status));
	
	if (it == existing_status.end())
		return UNKNOW_STATUS;
	else
	{
		return existing_status.find(first_line.status)->second;
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
		parser::entries block = pars.get_block("types", "mime").conf;
		if (block.find(subtype) != block.end())
			type = block.find(subtype)->second[0];
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

std::string response::find_charset(const request &req) const
{
	header_type::const_iterator it_tag;
	header_type head(req.get_header());
	if ((it_tag = head.find(ACCEPT_CHARSET)) == head.end())
		return "";

	std::multimap<int, std::string> map(tag_priority(it_tag->second)); //a type could be interesting
	std::multimap<int, std::string>::const_reverse_iterator it(map.rbegin()); //a type could be interesting
	std::multimap<int, std::string>::const_reverse_iterator end(map.rend());

	if (it == end)
		return "";
	else
		return it->second;
}

/*find if there is equivalent file for a specific language if there is, add content-language header field to header*/
std::string response::find_language(const std::string &complete_path, const request &req)
{
	header_type::const_iterator it_tag;

	if ((it_tag = req.get_header().find(ACCEPT_LANGUAGE)) == header.end())
		return complete_path;

	std::multimap<int, std::string> map(tag_priority(it_tag->second)); //a type could be interesting
	std::multimap<int, std::string>::const_reverse_iterator it(map.rbegin()); //a type could be interesting
	std::multimap<int, std::string>::const_reverse_iterator end(map.rend());
	struct stat file_stat; //information about file

	while (it != end)
	{
		std::string new_path(complete_path + "." + it->second);
		if (stat(new_path.c_str(), &file_stat) < 0)
		{
			if (!(errno == ENOENT)) //file doesnt exist or new_path is empty chain
				return ""; //error do something else
		}
		else
		{
			add_content_language(it->second);
			return new_path;
		}
		++it;
	}
	return complete_path;
}