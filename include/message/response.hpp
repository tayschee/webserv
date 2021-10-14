#ifndef RESPONSE_HPP
# define RESPONSE_HPP

class request;

# include "parser.hpp"
# include "message/message.hpp"
# include "message/request.hpp"
# include "message/exchange.hpp"
# include "server.hpp"
# include <dirent.h>
# include <sys/stat.h>
# include "utils.hpp" //files_in_dir

# include <list>
# include <map>

class parser;
//class request;

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
		typedef int (response::*method_function)(const std::string &path, const request &req, const parser &pars);

		//map type to find information
		typedef std::map<std::string, method_function>	method_array;
		typedef std::map<std::string, std::string>		media_type_array;
		typedef std::map<int, std::string>				status_array;

	private :
		response_line	first_line; //information about first line of response`
		std::string		func;
		std::string		save_path;
		int				&fdbody;
		int				&fdout;
		// int				pipe_in[2];
		const parser			*save_pars;


	private :
		static response::method_array			initialise_existing_method();
		const static method_array				existing_method; //std::map<std::string, method_function> > to store existing method and find appropriate function

	public :
		static response::status_array			initialise_existing_status();
		const static status_array				existing_status; //std::map<int, std::string> to store status_string and find it with status

	private : //utils
		std::string		header_to_string() const; //convert header to a string which be merge with other string to form response message
		void			main_header(const std::vector<std::string> &allow_method);
		void			main_header();
		std::string		header_first_line() const;
		bool			is_acces(const struct stat &file) const;
		bool			is_cgi(const std::string &type, const parser &pars) const;
		int				is_authorize(const std::string &path_str, const request &req, const parser &pars) const;
		void			status_header(int status, const std::string &path, const parser &pars);
		std::string		&file_without_language_ext(std::string &path) const; //maybe put path in const
		int				del_content(std::string path, const request &req, const parser &pars, const bool del = 1);
		int				check_path(const std::string & path, struct stat &file_stat, const request &req, const parser &pars) const;
		std::string		index(const std::string &path, std::string root, std::string add) const;
		int				generate_response(const parser::entries &path_info, const parser &pars, const request &req, const method_function &method);
		bool            is_cgi(const std::string &type, const parser &pars, const std::string &method) const;
		std::string		header_in_order(const std::string &hf_sep, const std::string &eol, const std::vector<std::string> &list) const;
		void			end_header(const request &req);

	private : //find_* functions, they return a value with a key without map
		/*the key_array allow_method is pass in parameter and create in response(std::string[3], header_type, body) in public.cpp*/
		method_array::mapped_type				find_method_function(const request &req, const std::vector<std::string> &allow_method, const parser &pars) const; //KEY : method, VALUE : function
		status_array::value_type::second_type	find_status_string(const int status) const; //KEY : status, VALUE: message
		//std::string								find_path(const parser::block &block, const std::string &partial_path,  const request &req) const;
		media_type_array::value_type			find_media_type(const std::string subtype) const; //KEY : subtype, VALUE : TYPE
		std::string								find_media_type(const std::string subtype, const parser &pars) const; //KEY : subtype, VALUE : TYPE
		std::string								find_path(const parser::block &block, const std::string &partial_path, const request &req, const bool index = 1) const;
	
		std::string								find_index(const parser::entries &entries, const std::string &path) const;
		const parser::address_conf::const_iterator	find_parser(const parser::address_conf &pars_list, const request &req) const;
		
		// const parser::address_conf::const_iterator	find_parser(const std::vector<parser::address_conf>::const_iterator &pars_list, const request &req) const;

	private : //method_is_* function, apply one of method
		int										method_is_head(const std::string &uri, const request &req, const parser &pars); //HEAD
		int										method_is_get(const std::string &uri, const request &req, const parser &pars); //GET
		int										method_is_delete(const std::string &uri, const request &req, const parser &pars); //DELETE
		int										method_is_options(const std::string &uri, const request &req, const parser &pars); //OPTION
		int										method_is_put(const std::string &uri, const request &req, const parser &pars); //PUT
		int										method_is_post(const std::string &uri, const request &req, const parser &pars); //POST
		int										method_is_trace(const std::string &uri, const request &req, const parser &pars); //POST
		int										method_is_connect(const std::string &uri, const request &req, const parser &pars); //CONNECT
		int										method_is_unknow(const std::string &uri, const request &req, const parser &pars); //UNKNOW

	private : //add_* functions, add something inside class like header_field or body
		void				add_allow(const std::vector<std::string> &allow_method_array); //Allow
		void				add_date(); //Date
		void				add_content_length(const off_t &bytes_size); //Content-Length
		void				add_last_modified(time_t time); //Last-Modified
		void				add_server(); //Server
		void				add_content_type(const std::string &file, const request &req); //Content-type
		void				add_content_type(const std::string &file); //Content-type without precise charset
		void				add_content_language(const std::string &language); //Content-Language
		void				add_transfert_encoding(); //Transfert-Encoding
		void				add_www_autentificate(const parser &pars, const std::string &path); //WWW-Authentificate
		void				add_retry_after(size_t sec); //Retry-After
		void				add_connection(int status, const request &req);
		int					add_body(const std::string &path);



	public :
		void 									parse_start_line(const std::string &start_line){ (void)start_line; }
		void 									parse_header(const std::string &start_line){ (void)start_line; }

	public : //get_* functions, inside getter.cpp, this function are used to have access private variable

		const response_line						&get_first_line() const;
		int										get_status() const;
		const std::string						&get_status_string() const;
		const std::string						&get_version() const;
		const std::string						&get_func() const;
		const std::string						&get_save_path() const;
		const parser							*get_save_pars() const;

		int										is_redirect(const parser::entries &block, const parser &pars, const request &req);

		std::string								get(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF)) const;
		std::string								get_rep_body(const std::string &eol = std::string(CRLF)) const;
		std::string								get_rep_header(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF)) const;

	private : //error_* functions, they are relations with error returns
		int										error_file(int errnum) const; //can maybe be change by find_* function
		void									default_error(int error_status, const request &req, const parser &pars);
		int										error_response(int status, const request &req, const parser &pars);
		int										error_response(int status, const request &req);
		int										redirect_to_error(const std::string &path, const request &req, const parser &pars);
		void									error_special_case(const request &req);

	public :

		// method_function method;


		response();
		response(const request &req, const parser::address_conf &pars_list, int &fdbody, int &fdout);
		response(int status, const request &req, const parser &pars, int &fdbody, int &fdout);

		// response(const request &req, const parser &pars, int status);
		response& operator=(const response &other);
		~response();
};

#endif