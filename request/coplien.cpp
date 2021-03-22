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

}

request::~request(){}