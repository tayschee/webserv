#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map> //std::map
# include <utility> //std::pair
# include <string> //std::string
# include <iostream> //ONLY TO TEST 

/*requete to send or receive*/
class request 
{
	private :
		struct			head_key
		{
			std::string	*key;

			head_key();
		}				head_key;

	private : //private variable
		std::string	header_key; //all type of information which must be inside the header
		std::map<std::string, std::string> header_data;
	public :
		request();
		//request(std::map header_data);
		~request();


};

#endif