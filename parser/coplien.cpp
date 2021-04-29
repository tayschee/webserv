#include "parser.hpp"

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

	return *this;
}

parser::block::block()
{
}

parser::block::block(const std::string &name, const std::vector<std::string> &args)
{
	this->name = name;
	this->args = args;
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
