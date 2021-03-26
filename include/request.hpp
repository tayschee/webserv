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
	public : //typedef
		typedef std::map<std::string, std::string>::const_iterator iterator;

	private : //private variable
		std::string							cmd; //first line of header with the commande the path and the version of http
		std::map<std::string, std::string>	header; //all information inside header after cmd
		std::string							body; //this is the body part of request
	public : //PRIVATE function which may be public for test
		std::string		time_string(time_t time_sec = time(NULL)) const; /*this function return date of today*/

	public :
		request();
		request(const char *txt);
		~request();
		std::string		get_cmd() const;
		iterator		begin() const; //return begin of header
		iterator		end() const; //return end of header

	/*if you dont compile with -D DEBUG=<value> those functions doesn't exist*/
	# if defined(DEBUG)
	public : //function to test request
		void print_cmd() const; //print cmd(first line of header)
		void print_header() const; //print all header_data (header without cmd)
		void print_body() const; // print body(body of request)
		void print_variable() const; //combination of three previous functions

	# endif

};

#endif