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
# include "cgi.hpp"
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
		response_line	first_line; //information about first line of response

	private :
		static response::method_array			initialise_existing_method();
		const static method_array				existing_method; //std::map<std::string, method_function> > to store existing method and find appropriate function

		static response::media_type_array		initialise_existing_media_type();
		const static media_type_array			existing_media_type; //std::map<std::string, std::string> to store existing type and find it with sub_type

	public :
		static response::status_array			initialise_existing_status();
		const static status_array				existing_status; //std::map<int, std::string> to store status_string and find it with status

	private : //utils
		std::string		header_to_string() const; //convert header to a string which be merge with other string to form response message
		void			main_header(const std::vector<std::string> &allow_method);
		void			main_header();
		std::string		header_first_line() const;
		std::multimap<int, std::string>	tag_priority(std::string tag) const;
		int				is_open(const struct stat &file) const;
		bool			is_cgi(const std::string &type, const parser &pars, const std::string &method) const;
		bool			is_authorize(const request &req, const parser &pars) const;
		void			status_header();
		std::string		&file_without_language_ext(std::string &path) const; //maybe put path in const
		int					del_content(std::string path, const request &req, const parser &pars, const bool del = 1);
		int					check_path(const std::string & path, struct stat &file_stat, const request &req, const parser &pars) const;
		std::string			index(const std::string &path, std::string root, std::string add) const;
		std::string			ft_itoa_base(long nb, std::string &base); //WARNING !!!!!!


	private : //find_* functions, they return a value with a key without map
		/*the key_array allow_method is pass in parameter and create in response(std::string[3], header_type, body) in public.cpp*/
		method_array::mapped_type				find_method_function(const std::string &method, const std::vector<std::string> &allow_method) const; //KEY : method, VALUE : function
		status_array::value_type::second_type	find_status_string() const; //KEY : status, VALUE: message
		//std::string								find_path(const parser::block &block, const std::string &partial_path,  const request &req) const;
		media_type_array::value_type			find_media_type(const std::string subtype) const; //KEY : subtype, VALUE : TYPE
		std::string								find_media_type(const std::string subtype, const parser &pars) const; //KEY : subtype, VALUE : TYPE
		std::string								find_path(const parser::block &block, const std::string &partial_path, const request &req, const bool index = 1) const;
		std::string								find_index(const parser::entries &entries, const std::list<std::string> &files) const;
		std::string 							find_charset(const request &req) const;
		std::string								find_language(const std::string &complete_path, const request &req);

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
		void				add_transfert_encoding(const std::string &file); //Transfert-Encoding
		void				add_www_autentificate(); //WWW-Authentificate
		void				add_retry_after(size_t sec); //Retry-After

		int					add_body(const std::string &path);


	public :
		void 									parse_start_line(const std::string &start_line){ (void)start_line; }
		void 									parse_header(const std::string &start_line){ (void)start_line; }

	public : //get_* functions, inside getter.cpp, this function are used to have access private variable

		const response_line					&get_first_line() const;
		int									get_status() const;
		const std::string					&get_status_string() const;
		const std::string					&get_version() const;

		//const std::string	&get_body() const;
		//const header_type	&get_header() const;

		void									get_code(const parser &pars);
		bool									is_redirect(parser::entries &block, const parser &pars);

		std::string								get(const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF)) const;
		int										sent(int fd, const std::string &hf_sep = std::string(": "), const std::string &eol = std::string(CRLF));

	private : //error_* functions, they are relations with error returns
		int										error_file(int errnum) const; //can maybe be change by find_* function
		void									default_error(int error_status, const request &req);
		int										error_response(int status, const request &req, const parser &pars);
		int										error_msg(const std::string &path, const request &req, const parser &pars);
		void									error_special_case(const request &req);

	public :
		response();
		response(const request &req, const parser &pars);
		response(const request::exception except, const parser &pars);
		~response();
};

#endif