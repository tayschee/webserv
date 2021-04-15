#ifndef REQUEST_HPP
# define REQUEST_HPP

# if defined(DEBUG)		//include for debug
#  include <iostream> 
# endif

# include <map>			//std::map
# include <utility>		//std::pair
# include <string>		//std::string
# include <sys/stat.h>	//stat
# include <fcntl.h>		//open 

# include "utils.hpp"	//is_horizontal_space()
# include "define.hpp"
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
		typedef std::pair<std::string, std::string>	value_type;

	public : //Class response for now in public but later in private to help me to create a function to send response that will be more intuituve
		class response
		{
			public : //response typedef
				//pointer to response function
				typedef int (response::*response_function)(const std::string &, const std::map<std::string, std::string> &, const std::string &);
				typedef request::iterator 		iterator;
				typedef request::header_type	header_type;
				typedef request::value_type		value_type;
				

			private :
				std::string		version; //version share by methode[VERSION]
				int				status; /*status code to know what's wrong or if all pass fine,
										it will be associated with a std::map to find name of status code*/
				header_type		header; /*header of response*/
				std::string		body;	/*body of response maybe put a fd instead*/

			private : //utils
				std::string		time_string(time_t time_sec = time(NULL)) const; /*this function return date of today or date specified*/
				std::string		header_to_string() const; //convert header to a string which be merge with other string to form response message
				void			main_header(const std::string *allow_method);
				std::string		header_first_line() const;
				void			error_response(const header_type &req_head);

			private : //get_* functions, they return a value with a key without map
				/*the key_array allow_method is pass in parameter and create in response(std::string[3], header_type, body) in public.cpp*/
				response_function	get_method_function(const std::string &method, const std::string *allow_method) const; //KEY : method, VALUE : function
				std::string			get_status_string() const; //KEY : status, VALUE: message
				value_type			get_media_type(const std::string subtype) const; //KEY : subtype, VALUE : TYPE

			private : //method_is_* function, apply one of method
				int					method_is_head(const std::string &file, const header_type &req_head, const std::string &req_body); //HEAD
				int					method_is_get(const std::string &file, const header_type &req_head, const std::string &req_body); //GET
				int					method_is_delete(const std::string &file, const header_type &req_head, const std::string &req_body); //DELETE
				int					method_is_options(const std::string &file, const header_type &req_head, const std::string &req_body); //OPTION
				int					method_is_put(const std::string &file, const header_type &req_head, const std::string &req_body); //PUT
				int					method_is_unknow(const std::string &file, const header_type &req_head, const std::string &req_body); //UNKNOW

			private : //add_* functions, add something inside class like header_field or body
				void				add_allow(const std::string *allow_method_array); //Allow
				void				add_date(); //Date
				void				add_content_length(const header_type &req_head, const off_t &bytes_size); //Content-Length
				void				add_last_modified(time_t time); //Last-Modified
				void				add_server(); //Server
				void				add_content_type(const std::string &file); //Content-type
				void				add_transfert_encoding(const std::string &file); //Transfert-Encoding

				void				add_body(int fd, struct stat file_stat); //body

			public :
				response(const std::string (&method)[3], const header_type &req_head, const std::string &req_body);
				~response();

				const std::string message() const;
			
			/*if you dont compile with -D DEBUG=<value> those functions doesn't exist*/
			# if defined(DEBUG)
			public : //function to test request
				void print_message();

			# endif
		};

	private : //private variable
		std::string							method[3];	//method arg and version
		header_type							header;		//all information inside header after method
		std::string							body;		//this is the body part of request
	public : //PRIVATE function which may be public for test
		void			method_parsing(const std::string &commande_line); /*use inside request(const char *txt) to parse method*/

	public :
		request();
		request(const char *txt);		//take in parameter the char * of receive or read to parse him
		~request();

		//void			wait();
		std::string		send_response() const;

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