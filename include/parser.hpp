#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <string>	 // std::string
#include <vector>	 // std::vector
#include <map>		 // std::map, std::multimap
#include <algorithm> // std::find_first_not
#include <iostream>	 // std::cerr
#include <fstream>	 // std::ifstream
#include <dirent.h>	 // opendir, closedir
#include <exception> // std::exception
#include "utils.hpp" // get_extension, clean_string

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

		block();
		block(const std::string &name, const std::vector<std::string> &args = std::vector<std::string>());
	};

private:
	typedef bool (parser::*check_func)(const std::vector<std::string>&, int) const;

	std::string filename;
	blocks _blocks;

private:

	parser(const std::string &_filename);

	void parse_file();
	void parse_line(std::string line, int line_no, blocks::key_type &block_id);

	bool is_valid() const;
	bool check_line(const std::string& line, int line_no) const;
	bool check_prop(const std::string& name, const std::vector<std::string>& args, int line_no) const;
	bool check_block(const std::string &name, const std::vector<std::string>& args, int line_no) const;
	bool check_prop_root(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_index(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_serv_name(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_return(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_accept(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_listen(const std::vector<std::string>& args, int line_no) const;
	bool check_prop_err_page(const std::vector<std::string>& args, int line_no) const;
	bool check_block_location(const std::vector<std::string>& args, int line_no) const;
	bool check_block_location_error(const std::vector<std::string>& args, int line_no) const;
	bool check_block_cgi(const std::vector<std::string>& args, int line_no) const;

	std::string find_best_match(std::string arg) const;
	std::string remove_comments(const std::string &line) const;

public:
	parser();
	parser(const parser &other);
	~parser();

	parser &operator=(const parser &other);

	static std::vector<parser> parse_folder(std::string path);
	static std::vector<std::string> split(const std::string &str, const std::string &delimiters = " \t");

	const block &get_block(const std::string &block_name, const std::vector<std::string> &block_args = std::vector<std::string>()) const;
	const block &get_block(const std::string &block_name, const std::string &block_arg) const;

	friend std::ostream &operator<<(std::ostream &os, const parser &parsed);
};

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);
std::ostream &operator<<(std::ostream &os, const parser::block &b);

#endif //WEBSERV_PARSER_HPP
