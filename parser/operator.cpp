#include "parser.hpp"

std::ostream& operator<<(std::ostream& os, const parser& parsed)
{
	os << "Filename : " << parsed.filename << std::endl;

	os << parsed.main;
	return os;
}

std::ostream& operator<<(std::ostream& os, const parser::block& b)
{
	os << "Block name : " << b.name << "(" << &b << ")" << std::endl;
	os << "Parent block address : " << b.parent << std::endl;

	os << "args :";
	if (b.args.empty())
		os << " none" << std::endl;
	else
	{
		os << " " << b.args[0];
		for (size_t i = 1; i < b.args.size(); i++)
			os << ", " << b.args[i];
		os << std::endl;
	}

	typedef std::map<std::string, std::vector<std::string> >::const_iterator const_iterator;
	for (const_iterator it = b.conf.begin(); it != b.conf.end(); it++)
	{
		os << " - " << it->first << " : " << it->second[0];
		for (size_t i = 1; i < it->second.size(); i++)
			os << ", " << it->second[i];
		if (b.blocks.size() || (++it)-- != b.conf.end())
			os << std::endl;
	}
	if (b.blocks.size())
		os << std::endl;
	for (size_t i = 0; i < b.blocks.size(); i++)
		os << *b.blocks[i];
	return os;
}
