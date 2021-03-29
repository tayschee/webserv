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

/*request sent*/
class request
{
	private :
		enum { METHOD, ARG, VERSION }; //To access element inside method with more explicit things than numbers

	public : //request typedef
		typedef std::map<std::string, std::string>::const_iterator iterator;
		typedef std::map<std::string, std::string>	header_type;

	public : //Class response for now in public but later in private to help me to create a function to send response that will be more intuituve
		class response
		{
			public : //response typedef
				typedef int (response::*response_function)(void); //pointeur to response function

			private :
				std::string		version; //version share by methode[VERSION]
				int				status; /*status code to know what's wrong or if all pass fine,
										it will be associated with map to find name of status code*/
				header_type		header; /*header of response*/
				std::string		body;	/*body of response maybe put a fd instead*/

			public : //later in private
				const response_function	get_method_function(const std::string &method, const std::string *allow_method) const;
				int						method_is_head();
				void					main_header(const std::string allow_method);
				void					add_allow(const std::string allow_method_array);
				void					add_date();	

			public :
				response(/*std::string (&method)[3], header_type &header, std::string &body*/);
				~response();

				//message_to_send();
		};

	private : //private variable
		std::string							method[3];	//method arg and version
		header_type							header;		//all information inside header after method
		std::string							body;		//this is the body part of request
	public : //PRIVATE function which may be public for test
		void			method_parsing(const std::string &commande_line); /*use inside request(const char *txt) to parse method*/
		std::string		time_string(time_t time_sec = time(NULL)) const; /*this function return date of today or date specified*/

	public :
		request();
		request(const char *txt);		//take in parameter the char * of receive or read to parse him
		~request();
		//std::string		response() const;

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