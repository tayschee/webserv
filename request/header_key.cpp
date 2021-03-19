#include "request.hpp"


/*	0.Accept-Charsets					9.Host								
	1.Accept-Language					10.Last-Modified
	2.Allow								11.Location							
	3.Authorization						12.Referer							All information to set inside header_key
	4.Content-Language					13.Retry-After						and their position inside std::string
	5.Content-Length					14.Server							header_key[18]
	6.Content-Location					15.Transfer-Encoding
	7.Content-Type						16.User-Agent
	8.Date								17.WWW-Authenticate
*/

request::head_key::head_key()
{

    const std::string[18] = {"Accept-Charsets", "Accept-Language", "Allow", "Authorization", "Content-Language", "Content-Length",
                        "Content-Location", "Content-Type", "Date", "Host", "Last-Modified", "Location", "Referer", "Retry-After",
                        "Server", "Transfer-Encoding", "User-Agent", "WWW-Authenticate"}
    key = new std::string(18);
    key = 


    
}