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
	Not optimised*/
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
		if (i - str_end > 1 && i != size)
		{
			str.erase(str_end + 1, i - str_end - 1);
			size = str.size();
			str_end = size;
			i = 0;
		}
	}
	str.erase(str_end);
	return str;
}

std::vector<std::string> split(const std::string &str, const std::string &delimiters)
{
	std::vector<std::string> result;

	size_t beg, pos = 0;
	while ((beg = str.find_first_not_of(delimiters, pos)) != std::string::npos)
	{
		pos = str.find_first_of(delimiters, beg + 1);
		result.push_back(str.substr(beg, pos - beg));
	}
	return result;
}
std::string ft_itoa(int nb)
{
	std::string str;
	long n = nb;
	bool sign = n < 0;

	n = sign ? n * -1 : n;
	while (n)
	{
		str.insert(str.begin(), n % 10 + '0');
		n /= 10;
	}
	return (sign ? "-" : "") + str;
}

size_t		ft_strlen(const char *str)
{
	size_t i(0);
	if (str == NULL)
		return i;
	while (str[i] != 0)
	{
		++i;
	}
	return i;
}

/*std::string get_extension(const std::string& str)
{
	std::string::size_type pos = str.find('.');

	return pos == str.npos ? "" : str.substr(pos);
}*/
