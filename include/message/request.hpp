#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "parser.hpp"
# include "message/message.hpp"
# include "message/exchange.hpp"
# include <vector>

class parser;
class response;

class request : public message
{
	public :
		struct	request_line //maybe do an inheritance
		{
			std::string	method;
			std::string uri;
			std::string query_string;
			std::string version;

			void	clear();
		};

	public :
		class exception : public std::exception
		{
			private :
				int 			status;
				std::string 	method;
			
			public :
				virtual const char	*what() const throw();
				const std::string	&get_method() const;
				int					get_status() const;

			public :
				exception(int status = 500, const std::string &method = "");
				exception(const exception &x);
				exception &operator=(const exception &x);
				virtual ~exception() throw();
		};

	public :
		typedef std::vector<std::string> method_array;

	public : //public variable
		static const method_array		existing_method;
	private :
		static method_array initialise_existing_method(); //to initialise method_array

	private : //private variable
		request_line						first_line;	//method arg and version
		//header_type							header;		//this is header part of request under form of std::map<std::String, std::string>
		//std::string							body;		//this is the body part of request

	private :
		void			parse_start_line(const std::string &start_line); /*use inside parse_header to determine first_line*/
		void			parse_header(const std::string &header_str);
		//void			parse_body()

	public : //getter.cpp, this function are used to have access information of the class
		const request_line	&get_first_line() const;

		const std::string	&get_method() const;
		const std::string	&get_uri() const;
		const std::string	&get_version() const;
		const std::string	get_secret() const;
		const std::string	get_content_type() const;
		const std::string	get_auth_type() const;
		const std::string	get_user() const;
		const std::string	get_content_length() const;
		const std::string	get_host() const;
		const std::string	get_query() const;
		const std::string	get_tf() const;

		//const std::string	&get_body() const;
		//const header_type	&get_header() const;

		response		get_response(const std::vector<parser::address_conf>::const_iterator pars) const; //to have response object
		std::string		get(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF)) const; //to have complete request of the form of std::string

	public :
		void			clear();
		int				check_path(const std::string &path) const;

	public :
		request();
		request(const exception &except);
		request(const std::string &const_request_str); //take in parameter the char * of receive or read to parse him
		//request(const request &x);
		request operator=(const request &x);
		~request();

		//int				receive(const int socket, receive_management &recv_data);
		int				validity() const;
};

#endif