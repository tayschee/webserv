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

/*parse header which were read*/
void			request::parse_header(const std::string &header_str)
{
	size_t pos;
	std::string str_key;

	size_t i = 1;
	std::vector<std::string> sub_header = split(header_str, CRLF); //split header_field

	if (sub_header.size()) //if sub_header exist
	{
		//put first line of header in special variables because it has not same syntax
		method_parsing(sub_header[0]); //WARNING all white space not check (tab)

		while (i < sub_header.size()) //while sub_header[i] exist
		{
			pos = sub_header[i].find(":"); //find return string::npos() if there is no ":"
			if (pos == sub_header[i].npos) //verify if there is ":"
			{
				//there is no ":"
				sub_header[i] = sub_header[i].substr(0, sub_header[i].size());
				header[clean_string(sub_header[i])] = "";
			}
			else
			{
				//there is ":"
				str_key = sub_header[i].substr(0, pos);
				sub_header[i] = sub_header[i].substr(pos + 1);
				header[clean_string(str_key)] = clean_string(sub_header[i]);
			}
			++i;
		}
	}
}

/* header is read, now read end of body */
void			request::read_end_of_body(const int socket)
{
	iterator end = header.end();
	iterator it(header.find(CONTENT_LENGTH));

	if (it != end) //is there a body ? 
	{
		//there is body
		size_t body_total_size = stoi(it->second);

		if (body.size() != body_total_size) //is there again something to read ?
		{
			//yes
			size_t buf_size = stoi(it->second) - body.size();
			char buffer[buf_size + 1];

			read(socket, buffer, buf_size);
			buffer[buf_size] = 0;
			body += buffer;
		}
	}
}

