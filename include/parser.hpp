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
		struct block
		{
			std::string name;
			std::vector<std::string> args;
			std::map<std::string, std::vector<std::string> > conf;
			block *parent;
			std::vector<block *> blocks;

			block(const std::string &name, const std::vector<std::string> &args = std::vector<std::string>(), block *parent = NULL);

			void add_block(block *b);
		};

	private:
		std::string filename;
		block main;

	private:
		parser();

		parser(const std::string &_filename);

		void parse_file();
		void parse_block(std::ifstream &ifs, block *b, int &number);

	public:
		parser(const parser &other);
		~parser();

		parser &operator=(const parser &other);

		static std::vector<parser *> parse_folder(std::string path);
		static std::vector<std::string> split(const std::string &str, const std::string &delimiters = " \t");

		friend std::ostream& operator<<(std::ostream& os, const parser& parsed);
};

std::ostream& operator<<(std::ostream& os, const parser::block& b);

#endif //WEBSERV_PARSER_HPP
