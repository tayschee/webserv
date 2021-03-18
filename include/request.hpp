#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <map> //std::map
#include <utility> //std::pair

/*requete to send or receive*/
class request 
{
	private : //private variable
		const	std::string	header[18];
		std::map<std::string, std::string> content;
	public :
		request();
		request(std::map content);


};

#endif