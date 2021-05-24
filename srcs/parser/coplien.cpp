#include "parser.hpp"
#include "message/request.hpp"

parser::parser()
{
}

parser::parser(const parser &other)
{
	*this = other;
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

parser::block::block()
{
}

parser::block::block(const std::string &name, const std::vector<std::string> &args)
{
	this->name = name;
	this->args = args;
	if (name == PARSER_SERVER)
	{
		this->conf.insert(std::pair<std::string, std::vector<std::string> >(PARSER_ACCEPT, request::existing_method));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >("port", std::vector<std::string>(1, "80")));
		this->conf.insert(std::pair<std::string, std::vector<std::string> >("keep_alive", std::vector<std::string>(1, "60")));
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
