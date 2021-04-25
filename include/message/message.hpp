#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# if defined(DEBUG)
#  include <iostream>
# endif

# include <map> 		//std::map
# include <vector>		//std::vector
# include <utility>		//std::pair
# include <string>		//std::string
# include <sys/stat.h>	//stat
# include <fcntl.h>		//open 
# include <errno.h>		//errno

# include "utils.hpp" 	//everything
# include "define.hpp" 	//everything
extern "C"				//use to add C library
{
	# include "libft.h" //ft_itoa but that must be change
}

class message
{
	public :
		typedef std::map<std::string, std::string>::const_iterator iterator;
		typedef std::map<std::string, std::string>	header_type;
		typedef std::pair<std::string, std::string>	value_type;
	
	protected :
		header_type		header;
		std::string		body;

	protected : //utils.cpp
		std::string		time_string(time_t time_sec = time(NULL)) const;

	protected : //parse.cpp
		void			parse_body(std::string &request_str);
	private :
		//virtual void 	parse_start_line(const std::string &start_line) = 0;
		//virtual void	parse_header(const std::string &header_str) = 0;

	public : //getter.cpp this function are used to have acess to private or protected variable
		header_type 	get_header() const;
		std::string 	get_body() const;

	public : //coplien.cpp
		message();
		message(header_type &header, std::string &body);
		message(message &x);

		//message operator=(message &x);
		~message();
};
#endif