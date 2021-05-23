#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "utils.hpp"

std::multimap<int, std::string>	tag_priority(std::string tag)
{
	const char tag_sep[] = ",";
	const char value_sep[] = ";q=";
	//tag.replace(0, tag.npos, " "); //for now white space do buf the code
	std::vector<std::string> split_tag(split(tag, tag_sep));
	std::vector<std::string>::const_iterator it(split_tag.begin());
	std::vector<std::string>::const_iterator end(split_tag.end());
	std::multimap<int, std::string>			 map;

	while (it < end)
	{
		std::cout << "ok\n";
		size_t pos;
		const std::string key_tag(*it);

		if ((pos = key_tag.find(value_sep)) != key_tag.npos)
		{
			map.insert(std::map<int, std::string>::value_type
			(ft_atoi<float>(key_tag.substr(pos + strlen(value_sep))) * 100, key_tag.substr(0, pos)));
		}
		else
		{
			map.insert(std::map<int, std::string>::value_type(1 * 100, key_tag));
		}
		++it;
	}
	return map;
}

int main(int c, char **v)
{
	if (c != 2)
	{
		std::cout << "need argument\n";
		return 1;
	}

	std::multimap<int, std::string> map(tag_priority(std::string(v[1])));
	std::multimap<int, std::string>::const_reverse_iterator it(map.rbegin());
	std::multimap<int, std::string>::const_reverse_iterator end(map.rend());

	while (it != end)
	{
		std::cout << it->first << ": " << it->second << "\n";
		++it;
	}
	return 0;
}