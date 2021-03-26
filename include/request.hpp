#ifndef REQUEST_HPP
# define REQUEST_HPP

# if defined(DEBUG)		//include for debug
#  include <iostream> 
# endif

# include <map>			//std::map
# include <utility>		//std::pair
# include <string>		//std::string
# include "utils.hpp"	//is_horizontal_space()
extern "C"				//use to add C library
{
	# include "libft.h" //ft_split
}

/*request to send*/
class request 
{
	private :
		enum { METHOD, ARG, VERSION }; //To access element inside method with more explicit things than numbers

	public : //typedef
		typedef std::map<std::string, std::string>::const_iterator iterator;

	private : //private variable
		std::string							method[3]; //method arg and version
		std::map<std::string, std::string>	header; //all information inside header after cmd
		std::string							body; //this is the body part of request
	public : //PRIVATE function which may be public for test
		void			method_parsing(const std::string &commande_line);
		std::string		time_string(time_t time_sec = time(NULL)) const; /*this function return date of today*/

	public :
		request();
		request(const char *txt);
		~request();
		iterator		begin() const; //return begin of header
		iterator		end() const; //return end of header
		std::string		execute();
		//char			*treatment() const;

	/*if you dont compile with -D DEBUG=<value> those functions doesn't exist*/
	# if defined(DEBUG)
	public : //function to test request
		void print_method() const; //print method(first line of header)
		void print_header() const; //print all header_data (header without cmd)
		void print_body() const; // print body(body of request)
		void print_variable() const; //combination of three previous functions

	# endif

};

#endif