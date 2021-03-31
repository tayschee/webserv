#include "parser.hpp"

parser::parser() : main("server")
{}

parser::parser(const parser& other) : main("server")
{
	*this = other;
}

parser::parser(const std::string& _filename) : filename(_filename), main("server")
{
	parse_file();
}

parser::~parser()
{
}

parser& parser::operator=(const parser& other)
{
	filename = other.filename;
	main = other.main;

	return *this;
}

parser::block::block(const std::string& name, const std::string& args, block *parent)
{
	this->name = name;
	this->args = args.empty() ? std::vector<std::string>() : split(args);
	this->parent = parent;
}
