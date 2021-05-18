#include <message/response.hpp>

/*Convert header_type to a syntax adapt for http do same thing than (std::string std::string::operator=(const header_type &)),
that doesn't exist for now, but it can be implement*/
std::string		response::header_to_string() const
{
	const_iterator it(header.begin());
	const_iterator end(header.end());
	std::string str;

	while (it != end)
	{
		str += it->first;
		str += ": ";
		str += it->second;
		str += CRLF;
		++it;
	}
	return str;
}

/*add inside response:header all field which are in all method and in all condition*/
void			response::main_header(const std::vector<std::string> &allow_method)
{
	add_allow(allow_method); // allow_field add in header
	add_date(); //date field add in header
	add_server(); //server field add in header
}

/*create first line of response header */ 
std::string		response::header_first_line() const
{
	std::string		str_first_line;

	str_first_line = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.version + CRLF;

	return str_first_line;
}

/*parse value of accept* header-field*/
std::multimap<int, std::string>	response::tag_priority(std::string tag) const
{
	const char tag_sep[] = ",";
	const char value_sep[] = ";q=";
	//tag.replace(0, tag.npos, " "); //must add replace function to delete alll white space
	std::vector<std::string> split_tag(split(tag, tag_sep));
	std::vector<std::string>::const_iterator it(split_tag.begin());
	std::vector<std::string>::const_iterator end(split_tag.end());
	std::multimap<int, std::string>			 map;

	while (it < end)
	{
		std::cout << "ok\n";
		size_t pos;
		const std::string key_tag(*it);

		if ((pos = key_tag.find(value_sep)) != key_tag.npos)
		{
			map.insert(std::map<int, std::string>::value_type
			(ft_atoi<float>(key_tag.substr(pos + strlen(value_sep))) * 100, key_tag.substr(0, pos)));
		}
		else
		{
			map.insert(std::map<int, std::string>::value_type(1 * 100, key_tag));
		}
		++it;
	}
	return map;
}
