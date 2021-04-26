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
	blocks = other.blocks;

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
