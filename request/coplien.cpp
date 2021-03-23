#include "request.hpp"
// defining all functions used for canonique mode.


/*	0.Accept-Charsets					9.Host = 127.0.0.1 or localhost							
	1.Accept-Language = en				10.Last-Modified
	2.Allow								11.Location							
	3.Authorization						12.Referer							All information to set inside header_key
	4.Content-Language					13.Retry-After						and their position inside std::string
	5.Content-Length = nb char			14.Server = nginx/1.19.18			header_key[18]
	6.Content-Location					15.Transfer-Encoding = extent of data?
	7.Content-Type = text/html			16.User-Agent
	8.Date								17.WWW-Authenticate
*/

request::request()
{
	/*std::map<std::string, std::string>::iterator it;
	size_t	i = 0;

	std::string
	while (i < 18) //18 must be change by "size of header_key"
	{
		header_data.insert(std::pair(header_key[i], "???"));
		++i;
	}
	it = header_data.begin();
	while (it != header_data.end())
		std::cout << it->first << " : " << it->second << "\n";*/
}

request::request(const char *txt)
{
	std::string	split_str(txt); //use to cut a string in two part
	std::string	header_str; //use to store header
	char		**sub_header; //use to store header with all lines separated
	size_t i = 1;

	/*split header and message of request*/ 
	i = split_str.find("\n\n");
	message = split_str.substr(i);
	header_str = split_str.erase(i);

	sub_header = ft_split(header_str.c_str(), '\n');

	/*put commande inside*/
	cmd = *(sub_header);

	/*for each element in sub_header put them inside map<std::string, std::string> to access each element individually WARNING i start by 1*/
	while (sub_header[i])
	{
		split_str = sub_header[i];
		i = split_str.find(":");
		header_data[split_str.substr(0, i)] = split_str.substr(i + 1); //WARNING must verify if we can +1 , +1 is to pass space
	}
	//to free allocation of 2 dimension for example char **
	free_malloc_2d(sub_header);
}

request::~request(){}