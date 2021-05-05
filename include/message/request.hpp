#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "message/message.hpp"
# include "message/response.hpp"
# include "../parser.hpp"

class response;

class request : public message
{
	public :
		struct	request_line
		{
			std::string	method;
			std::string uri;
			std::string version;
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

		std::string		get() const; //to have complete request of the form of std::string
		response		get_response(const parser &pars) const; //to have response object

	public :
		request();
		request(const char *request_char); //take in parameter the char * of receive or read to parse him

		//request(const request &x);
		request operator=(const request &x);
		~request();

		void            receive_cl_body(const int socket, receive_management *recv_data);
		void			check_end_of_cl_body(receive_management *recv_data);
		void			receive_tf_body(const int socket, const size_t buf_size);
		void			receive_cl_body(const int socket, const size_t buf_size);
		void			receive_body(const int socket, receive_management *recv_data, const size_t buf_size);
		void			prepare_receive_body(receive_management *recv_data, size_t pos);
		void			receive_header(const int socket, receive_management *recv_data, const size_t buf_size);
		bool			receive(const int socket, receive_management *recv_data, const size_t buf_size);

		//int			read_socket(const int socket);
		//std::string		send_response() const;

};

#endif