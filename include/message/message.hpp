#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# if defined(DEBUG)
#  include <iostream>
# endif

# include <map> 		//std::multimap std::map
# include <vector>		//std::vector
# include <utility>		//std::pair maybe inside map
# include <string>		//std::string
# include <sys/stat.h>	//stat
# include <fcntl.h>		//open 
# include <errno.h>		//errno
# include <unistd.h>	//write, read

# include "utils.hpp" 	//everything
# include "define.hpp" 	//everything

class message
{
	public :
		/*structure will be use to store information during receive and send process*/
		class exchange_management;	
		class receive_management;
		//class send_management;

	public :
		typedef std::multimap<std::string, std::string>		header_type;
		typedef header_type::iterator 						iterator;
		typedef header_type::const_iterator					const_iterator;
		typedef header_type::value_type						value_type;
	
	protected :
		header_type		header;
		std::string		body;

	protected : //utils.cpp
		std::string		time_string(time_t time_sec = time(NULL)) const;

	protected : //parse.cpp
		void			parse_body(std::string &request_str);
	private :
		virtual void 	parse_start_line(const std::string &start_line) = 0;
		virtual void	parse_header(const std::string &header_str) = 0;

	public : //getter.cpp this function are used to have acess to private or protected variable
		header_type 	get_header() const;
		std::string 	get_body() const;
		virtual std::string	get(const std::string &hf_sep = std::string(": "), const std::string &eol= std::string(CRLF)) const = 0;
	public : //coplien.cpp
		message();
		message(header_type &header, std::string &body);
		message(message &x);

		//message operator=(message &x);
		virtual ~message();
};

#endif