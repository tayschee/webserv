#include "request.hpp"
// defining all functions used for canonique mode.


/*
	0.Accept-Charsets					9.Host = 127.0.0.1 or localhost							
	1.Accept-Language = en				10.Last-Modified
	2.Allow								11.Location							
	3.Authorization						12.Referer								All informations which can contain header
	4.Content-Language					13.Retry-After
	5.Content-Length = nb char			14.Server = nginx/1.19.18
	6.Content-Location					15.Transfer-Encoding = extent of data?
	7.Content-Type = text/html			16.User-Agent
	8.Date								17.WWW-Authenticate
*/

request::request(const char *txt)
{
	std::string	split_str(txt);	//use to store a string will be cut in two part like txt will be cut into a header and a message
	std::string	str_key;		//string behind ":"
	char		**sub_header;	//use to store header with all lines separated
	size_t		pos;			//position of ":"
	size_t 		i = 1;			//incrementation, start after commande

	/*split header and message of request*/ 
	pos = split_str.find("\n\n");
	if (pos == split_str.npos) //verify if there is a body part
		return ;
	message = split_str.substr(pos + 2);

	sub_header = ft_split(split_str.substr(0, pos).c_str(), '\n');

	/*for each element in sub_header put them inside map<std::string, std::string> to access each element individually*/
	if (sub_header && *sub_header)
	{
		cmd = *sub_header; //put first commande in special variable cmd because she is not store like other

		i = 1;
		while (sub_header[i])
		{
			split_str = sub_header[i];
			pos = split_str.find(":"); //find return string::npos() if there is no ":"
			if (pos == split_str.npos) //verify if there is ":"
				header[split_str.substr(nb_horizontal_space(split_str), split_str.size())] = "";
			else
			{
				str_key = split_str.substr(0, pos);
				split_str = split_str.substr(pos + 1);
				header[clean_string(str_key)] = clean_string(split_str);
			}
			++i;
		}
	}
	//to free allocation of char **
	free_malloc_2d(sub_header);

	//why not throw exception if there is a probleme
}

request::~request(){}