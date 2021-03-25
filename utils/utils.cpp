#include "utils.hpp"

bool	is_horizontal_space(const int c) //verify if c is space or tab
{
	if (c == 32 || c == 9)
		return 1;
	return 0;
}

size_t	nb_horizontal_space(const std::string &str) //return the number of successive horizontal space
{
	size_t i = 0;

	while (is_horizontal_space(str[i]))
		++i;
	return i;
}

/*	delete useless white space at the begin and at the end of the string.
	Possible improuvement with delete useless white space inside string */
std::string clean_string(std::string &str)
{
	size_t	str_begin = 0; //how many element to pass space in begin of string
	size_t	str_end; //how many element to pass space in end of string
	size_t	size; //string size
	size_t	i = 0; //inc
	
	str_begin = nb_horizontal_space(str);
	str.erase(0, str_begin);
	size = str.size();
	str_end = size;

	while (i < size)
	{
		while (i < size && !is_horizontal_space(str.c_str()[i]))
			++i;
		str_end = i;
		i += nb_horizontal_space(&str.c_str()[i]);
	}
	str.erase(str_end, i);
	return str;
}