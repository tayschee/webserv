#include "message/request.hpp"

/* Store information about request (cmd, arg and version)*/
void			request::parse_start_line(const std::string &start_line)
{
	std::vector<std::string> cmd_split;

	cmd_split = split(start_line.c_str(), " "); //NEED FT_SPLIT PISCINE to verify tab + space

	first_line.method = cmd_split[0];
	first_line.version = cmd_split[2];
	cmd_split = split(cmd_split[1], "?");
	first_line.uri = cmd_split[0];
	if (cmd_split.size() > 1)
		header.insert(value_type(QUERY_STRING, cmd_split[1]));
}

/*parse header which were read*/
void			request::parse_header(const std::string &header_str)
{
	size_t i;
	size_t pos;
	std::string str_key;
	std::vector<std::string> sub_header = split(header_str, CRLF); //split header_field

	if (sub_header.size()) //if sub_header exist
	{
		//put first line of header in special variables because it has not same syntax
		parse_start_line(sub_header[0]); //WARNING all white space not check (tab)

		for (i = 1; i < sub_header.size(); i++) //while sub_header[i] exist
		{
			pos = sub_header[i].find(":"); //find return string::npos() if there is no ":"
			if (pos == sub_header[i].npos) //if there is not ":"
			{
				header.insert(value_type(clean_string(sub_header[i]), ""));
			}
			else //if there is ":"
			{
				str_key = sub_header[i].substr(0, pos);
				sub_header[i].erase(0, pos + 1);
				header.insert(value_type(clean_string(str_key), clean_string(sub_header[i])));
			}
		}
	}
}