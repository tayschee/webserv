#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "parser.hpp"
# include "message/message.hpp"
# include "message/response.hpp"
# include "message/exchange_management.hpp"

class response;

class request : public message
{
	public :
		struct	request_line //maybe do an inheritance
		{
			std::string	method;
			std::string uri;
			std::string version;

			void	clear();
		};

	private : //private variable
		request_line						first_line;	//method arg and version
		//header_type							header;		//this is header part of request under form of std::map<std::String, std::string>
		//std::string							body;		//this is the body part of request

	private :
		void			parse_start_line(const std::string &start_line); /*use inside parse_header to determine first_line*/
		void			parse_header(const std::string &header_str);
		//void			parse_body()

	private :
		//void			read_end_of_body(const int socket);

	public : //getter.cpp, this function are used to have access information of the class
		request_line	get_first_line() const;

		std::string		get_method() const;
		std::string		get_uri() const;
		std::string		get_version() const;
		//std::string	get_body() const;
		//header_type	get_header() const;

		response		get_response(const parser &pars) const; //to have response object
		std::string		get(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF)) const; //to have complete request of the form of std::string

	public :
		void			clear();

	public :
		request();
		request(const char *request_char); //take in parameter the char * of receive or read to parse him
		//request(const request &x);
		request operator=(const request &x);
		~request();

		bool			receive(const int socket, receive_management &recv_data);

		//int			read_socket(const int socket);
		//std::string		send_response() const;

};

#endif