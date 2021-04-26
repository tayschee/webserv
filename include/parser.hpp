#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <string>	 // std::string
#include <vector>	 // std::vector
#include <map>		 // std::map
#include <algorithm> // std::find_first_not
#include <iostream>	 // std::cerr
#include <fstream>	 // std::ifstream
#include <dirent.h>	 // opendir, closedir

class parser
{
public:
	typedef std::map<std::string, std::vector<std::string> > entries;

	struct block
	{
		std::string name;
		std::vector<std::string> args;
		entries conf;

		block();
		block(const std::string& name, const std::vector<std::string> &args = std::vector<std::string>());
	};

private:
	std::string filename;
	std::map<std::string, block> blocks;

private:
	parser();

	parser(const std::string &_filename);

	void parse_file();
	void parse_line(std::string line, int line_no, std::string& block_id);

public:
	parser(const parser &other);
	~parser();

	parser &operator=(const parser &other);

	static std::vector<parser> parse_folder(std::string path);
	static std::vector<std::string> split(const std::string &str, const std::string &delimiters = " \t");

	const entries &operator[](const std::string& block_name) const;
	const block &get_block(const std::string& block_name) const;

	friend std::ostream& operator<<(std::ostream& os, const parser& parsed);
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v);
std::ostream& operator<<(std::ostream& os, const parser::block& b);

#endif //WEBSERV_PARSER_HPP
