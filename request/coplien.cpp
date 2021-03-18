#include "request.hpp"

// defining all functions used for canonique mode.


/*	0.Accept-Charsets					9.Host								
	1.Accept-Language					10.Last-Modified
	2.Allow								11.Location							
	3.Authorization						12.Referer							All information to set inside header
	4.Content-Language					13.Retry-After						and their posiion inside std::string
	5.Content-Length					14.Server							header[18]
	6.Content-Location					15.Transfer-Encoding
	7.Content-Type						16.User-Agent
	8.Date								17.WWW-Authenticate
*/
request::request() : header[0]("Accept-Charsets"), header[1]("Accept-Language"), header[2]("Allow"), header[3]("Authorization"),
header[4]("Content-Language"), header[5]("Content-Length"), header[6]("Content-Location"), header[7]("Content-Type"),
header[8]("Date"), header[9]("Host"), header[10]("Last-Modified"), header[11]("Location"), header[12]("Referer"),
header[13]("Retry-After"), header[14]("Server"), header[15]("Transfer-Encoding"), header[16]("User-Agent"),
header[17]("WWW-Authenticate")
{
	size_t	i = 0;

	while (i < header.size())
	{
		content.insert(std::pair(header[i], "???"));
		++i;
	}
}