#ifndef UTILS_HPP
# define UTILS_HPP

# include <string> //size_t, std::string
# include <vector> //std::vector


bool		is_horizontal_space(int c); //return 1 for space and tab and 0 for newline vertical tab and all other...
size_t		nb_horizontal_space(const std::string &str); //return number of succesive horizontal_space
std::string	clean_string(std::string &str); //delete useless horizontal space in a string
std::vector<std::string> split(const std::string &str, const std::string &delimiters); //split string with delimiters

std::string				ft_itoa(const int nb);
size_t					ft_strlen(const char *str);
template<class T> 	T	ft_atoi(const std::string &str)
{
	size_t	i(0);
	T	nb(0);

	if (str.size() != 0)
	{
		if (str[0] == '-')
		{
			nb *= -1;
			++i;
		}
		else if (str[0] == '+')
			++i;
		while (i < str.size() && str[i] >= '0' && str[i] <= '9')
		{
			nb = 10 * nb + str[i] - '0';
			++i;
		}
	}
	return nb;
}

/*why not do same function for vertical space*/


#endif
