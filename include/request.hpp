#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map> //std::map
# include <utility> //std::pair
# include <string> //std::string
# include <iostream> //ONLY TO TEST 
# include "libft.h" //libft

/*requete to send or receive*/
class request 
{
	private :
		/*struct			head_key
		{
			std::string	*key;

			head_key();
		}				head_key;*/

	private : //private variable
		//std::string	header_key; //all type of information which must be inside the header
		std::string							cmd;
		std::map<std::string, std::string>	header_data;
		std::string							message;
	public : //private function which may be public for test
		std::string		time_string(time_t time_sec = time(NULL)) const; /*this function return date of today*/
	public :
		request();
		request(const char *txt);
		//request(std::map header_data);
		~request();


};

#endif