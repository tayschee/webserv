#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <string>	 // std::string
#include <vector>	 // std::vector
#include <map>		 // std::map, std::multimap
#include <algorithm> // std::find_first_not
#include <iostream>	 // std::cerr
#include <fcntl.h>	 // open, close
#include <errno.h>	 // errno
#include <cstring>	 // strerror
#include <unistd.h>	 // read, write
#include <dirent.h>	 // opendir, closedir
#include <exception> // std::exception
#include "utils.hpp" // get_extension, clean_string

#define BUFFER_SIZE 3200

// TODO: move these defines to define.hpp
#define PARSER_SERVER "server"
#define PARSER_LOCATION "location"
#define PARSER_TYPES "types"
#define PARSER_CGI "cgi"

#define PARSER_ACCEPT "accept"
#define PARSER_LISTEN "listen"
#define PARSER_HOST "host"
#define PARSER_ERROR_PAGE "error_page"
#define PARSER_INDEX "index"
#define PARSER_AUTOINDEX "autoindex"
#define PARSER_ROOT "root"
#define PARSER_SCRIPT_NAME "script_name"
#define PARSER_KEEP_ALIVE "keep_alive"
#define PARSER_RETURN "return"
#define PARSER_AUTH_BASIC "auth_basic"
#define PARSER_AUTH_BASIC_USER_FILE "auth_basic_user_file"
#define PARSER_BODY_SIZE_MAX "body_size_max"
#define PARSER_SERVER_NAME "server_name"
#define PARSER_ALIAS "alias"

class parser
{
public:
	struct block;

	typedef std::map<std::string, std::vector<std::string> > entries;
	typedef std::map<std::pair<std::string, std::vector<std::string> >, block> blocks;

	class BlockNotFound : public std::exception
	{
		public:
			BlockNotFound(const std::string& name, const std::vector<std::string>& args) throw();
			~BlockNotFound() throw();

			const char *what() const throw();

		private:
			std::string err;
			const std::string& name;
			const std::vector<std::string>& args;
	};

	struct block
	{
		std::string name;
		std::vector<std::string> args;
		entries conf;

		std::map<int, std::string> errors;

		block();
		block(const std::string &name, const std::vector<std::string> &args = std::vector<std::string>());
		block	&operator=(const block &x);
		void	create_block(const std::string name,  const std::vector<std::string> &args, const block &serv_block);
	};

private:
	typedef bool (parser::*check_prop_func)(const std::string&, const std::vector<std::string>&, int) const;
	typedef bool (parser::*check_block_func)(const std::vector<std::string>&, int) const;

	struct ServerNameEntry
	{
		std::string filename;
		int line_no;
		std::string host;
		std::string port;
	};

	std::string filename;
	blocks _blocks;
	static std::string buffer;
	bool error;
	static block mime;

	static std::map<std::string, ServerNameEntry> names; // server_name -> (filename, line)

private:
	parser(const std::string &_filename);

	static block	create_default_mime_type();

	static bool getline(int fd, std::string& line);
	void parse_file();
	void parse_line(std::string line, int line_no, blocks::key_type &block_id);
	static void parse_mime(const std::string& path); // get the block with get_mime()
	static void parse_line_mime(const std::string& filename, const std::string& line, int line_no);
	//std::string

	bool basic_chk_block(const std::string& name, const std::string& actual, const std::vector<std::string>& expected, int line_no) const;
	bool basic_chk_args(const std::string& name, int actual, int expected, bool exact, int line_no) const;
	bool advanced_chk_err_code(const std::vector<std::string>& err, int line_no) const;

	bool validate();
	bool check_line(const std::string& line, int line_no) const;
	bool check_prop(const std::string& name, const std::string& block_id, const std::vector<std::string>& args, int line_no);
	bool check_block(const std::string &name, const std::vector<std::string>& args, int line_no);
	bool check_prop_root(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_index(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_host(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_accept(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_listen(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_err_page(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_autoindex(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_script_name(const std::string& block_id, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_return(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_auth_basic(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_auth_basic_user_file(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_keep_alive(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_serv_name(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_body_size_max(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_prop_alias(const std::string &block_id, const std::vector<std::string> &args, int line_no) const;
	bool check_block_location(const std::vector<std::string>& args, int line_no) const;
	bool check_block_cgi(const std::vector<std::string>& args, int line_no) const;

	std::string find_best_match(std::string arg) const;
	static std::string remove_comments(const std::string &line);

	void add_property(const blocks::key_type& block_id, const std::string& name, const std::vector<std::string> &splitted, int line_no);

public:
	parser();
	parser(const parser &other);
	~parser();

	parser &operator=(const parser &other);

	static std::vector<parser> parse_folder(std::string path);
	static std::vector<std::string> split(const std::string &str, const std::string &delimiters = " \t");

	const block &get_block(const std::string &block_name, const std::vector<std::string> &block_args = std::vector<std::string>()) const;
	const block &get_block(const std::string &block_name, const std::string &block_arg) const;

	static const block& get_mime();

	friend std::ostream &operator<<(std::ostream &os, const parser &parsed);
};

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);
std::ostream &operator<<(std::ostream &os, const parser::block &b);

void append_buffer(std::string &buffer, std::string &line, size_t limit, bool resize);

#endif //WEBSERV_PARSER_HPP
