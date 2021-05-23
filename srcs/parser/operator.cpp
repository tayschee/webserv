#include "parser.hpp"

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v)
{
	if (v.empty())
		return os << "empty";

	os << v[0];
	for (size_t i = 1; i < v.size(); i++)
		os << ", " << v[i];
	return os;
}

std::ostream &operator<<(std::ostream &os, const parser::block &b)
{
	os << "Name: " << b.name << std::endl;
	os << "Args: " << b.args << std::endl;

	os << "Conf: " << std::endl;
	for (parser::entries::const_iterator it = b.conf.begin(); it != b.conf.end(); it++)
		os << " - " << it->first << " -> " << it->second << std::endl;

	os << "Errors: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = b.errors.begin(); it != b.errors.end(); it++)
		os << " - " << it->first << " -> " << it->second << std::endl;
	return os;
}

std::ostream &operator<<(std::ostream &os, const parser &parsed)
{
	os << "Filename : " << parsed.filename << std::endl;

	bool validity = parsed.is_valid();
	os << "Validity : " << (validity ? "\033[32;1mTrue" : "\033[31;1mFalse") << "\033[0m" << std::endl;

	if (!validity)
		return os;
	for (parser::blocks::const_iterator it = parsed._blocks.begin();
		 it != parsed._blocks.end(); it++)
	{
		os << it->second;

		if ((++it)-- != parsed._blocks.end())
			os << std::endl;
	}

	return os;
}
