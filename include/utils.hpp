#ifndef UTILS_HPP
# define UTILS_HPP

# include <string> //size_t


bool		is_horizontal_space(int c); //return 1 for space and tab and 0 for newline vertical tab and all other...
size_t		nb_horizontal_space(const std::string &str); //return number of succesive horizontal_space
std::string	clean_string(std::string &str); //delete useless horizontal space in a string


#endif