#include "parser.hpp"

parser::parser()
{}

parser::parser(const parser& other)
{
	*this = other;
}

parser::parser(const std::string& _filename) : filename(_filename)
{
	parse_file();
}

parser::~parser()
{}

parser& parser::operator=(const parser& other)
{
	filename = other.filename;
	blocks = other.blocks;

	return *this;
}
