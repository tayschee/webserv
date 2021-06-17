#include "parser.hpp"
#include "message/request.hpp"

std::string parser::buffer;
parser::block parser::mime;
std::map<std::string, parser::ServerNameEntry> parser::names;

parser::parser()
{
}

parser::parser(const parser &other)
{
	filename = other.filename;
	_blocks = other._blocks;
	error = other.error;
}

parser::parser(const std::string &_filename) : filename(_filename)
{
	parse_file();
}

parser::~parser()
{
}

parser &parser::operator=(const parser &other)
{
	filename = other.filename;
	_blocks = other._blocks;
	error = other.error;

	return *this;
}

parser::block &parser::block::operator=(const parser::block &x)
{
	this->name = x.name;
	this->args = x.args;

	if (name == PARSER_SERVER)
	{
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_ACCEPT, request::existing_method));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_KEEP_ALIVE, std::vector<std::string>(1, "60")));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_BODY_SIZE_MAX, std::vector<std::string>(1, "60")));
		//this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_AUTOINDEX, std::vector<std::string>(1, "off")));
	}
	return *this;
}

void parser::block::create_block(const std::string name, const std::vector<std::string> &args, const block &serv_block)
{
	this->name = name;
	this->args = args;

	if (serv_block.name == PARSER_SERVER)
	{
		if (this->name == PARSER_LOCATION)
		{
			this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_ACCEPT,
			serv_block.conf.find(PARSER_ACCEPT)->second));
		}
	}
}

parser::block::block(){}

parser::block::block(const std::string &name, const std::vector<std::string> &args)
{
	this->name = name;
	this->args = args;
	if (name == PARSER_SERVER)
	{
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_ACCEPT, request::existing_method));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_KEEP_ALIVE, std::vector<std::string>(1, "60")));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_BODY_SIZE_MAX, std::vector<std::string>(1, "60")));
		//this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_AUTOINDEX, std::vector<std::string>(1, "off")));
	}
}

parser::BlockNotFound::BlockNotFound(const std::string& n, const std::vector<std::string>& a) throw() : name(n), args(a)
{
	err = "Error: The block named " + name + " with ";

	if (args.empty())
		err += "no args";
	else
		err += "args " + args[0];
	for (size_t i = 1; i < args.size(); i++)
		err += ", " + args[i];
	err += ".";
}

parser::BlockNotFound::~BlockNotFound() throw()
{
}
