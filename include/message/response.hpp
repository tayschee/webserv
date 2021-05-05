#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "message/message.hpp"
#include  "message/request.hpp"
#include "../parser.hpp"

class request;

class response : public message
{
	public :
		struct response_line
		{
			int			status; //response status
			std::string	status_string; //response status string
			std::string version; //http version
		};

	public : //typedef
		//pointer to method_function
		typedef int (response::*method_function)(const request &req, const parser &pars);

		//map type to find information
		typedef std::map<std::string, method_function>	method_array;
		typedef std::map<std::string, std::string>		media_type_array;
		typedef std::map<int, std::string>				status_array;

	private :
		response_line	first_line; //information about first line of response
		//header_type		header; //header of response
		//std::string		body;	//body of response maybe put a fd instead

	private :
		static response::method_array	initialise_existing_method();
		const static method_array	existing_method; //std::map<std::string, method_function> > to store existing method and find appropriate function

		static response::media_type_array	initialise_existing_media_type();
		const static media_type_array	existing_media_type; //std::map<std::string, std::string> to store existing type and find it with sub_type

		static response::status_array	initialise_existing_status();
		const static status_array	existing_status; //std::map<int, std::string> to store status_string and find it with status

	private : //utils
		std::string		header_to_string() const; //convert header to a string which be merge with other string to form response message
		void			main_header(const std::string *allow_method);
		std::string		header_first_line() const;

	private : //find_* functions, they return a value with a key without map
		/*the key_array allow_method is pass in parameter and create in response(std::string[3], header_type, body) in public.cpp*/
		method_array::value_type::second_type	find_method_function(const std::string &method, const std::string *allow_method) const; //KEY : method, VALUE : function
		status_array::value_type::second_type	find_status_string() const; //KEY : status, VALUE: message
		media_type_array::value_type			find_media_type(const std::string subtype) const; //KEY : subtype, VALUE : TYPE

	private : //method_is_* function, apply one of method
		int					method_is_head(const request &req, const parser &pars); //HEAD
		int					method_is_get(const request &req, const parser &pars); //GET
		int					method_is_delete(const request &req, const parser &pars); //DELETE
		int					method_is_options(const request &req, const parser &pars); //OPTION
		int					method_is_put(const request &req, const parser &pars); //PUT
		int					method_is_unknow(const request &req, const parser &pars); //UNKNOW

	private : //add_* functions, add something inside class like header_field or body
		void				add_allow(const std::string *allow_method_array); //Allow
		void				add_date(); //Date
		void				add_content_length(const header_type &req_head, const off_t &bytes_size); //Content-Length
		void				add_last_modified(time_t time); //Last-Modified
		void				add_server(); //Server
		void				add_content_type(const std::string &file); //Content-type
		void				add_transfert_encoding(const std::string &file); //Transfert-Encoding

		bool				add_body(int fd, struct stat &file_stat); //body

	public : //get_* functions, inside getter.cpp, this function are used to have access private variable
		response_line	get_first_line() const;

		int				get_status() const;
		std::string		get_status_string() const;
		std::string		get_version() const;
	
		//std::string	get_body() const;
		//header_type	get_header() const;
		std::string		get(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF));
		void			get_error(int error, const parser &pars);

	private : //error_* functions, they are relations with error returns
		int					error_file(int errnum) const; //can maybe be change by find_* function

	public :
		response(const request &req, const parser &pars);
		~response();

		const std::string message() const;
		void	set_size()
		{
		//	header["Content-Length"] =  ft_itoa(body.size());
			header["Content-Type"] =  "text/html";
		}
};

#endif