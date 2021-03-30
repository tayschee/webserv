#include <request.hpp>

/*This file regroup all functions add_*, they are all private and are use to complete header header field by header field */

/*add header_field allow inside header with ", " beetween methods allowed */
void			request::response::add_allow(const std::string *allow_method_array)
{
	std::string allow_method_string;
	int i;

	for (i = 0; allow_method_array[i] != ""; i++) //can be optimised
	{
		allow_method_string += allow_method_array[i];
		if (allow_method_array[i + 1] == "")
			break ;
		allow_method_string += ", ";
	}



	header.insert(std::pair<std::string, std::string>("Allow", allow_method_string));
}

void			request::response::add_date()
{
	const std::string date = time_string();
	const std::string date_key = "Date";

	header.insert(std::pair<std::string, std::string>(date_key, date));
}