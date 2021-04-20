#include <request.hpp>

/*	this function return a date into the form of string which is similar to : "Mon, 12 Mar 2088 12:27:32 GMT"
	this function have a default argument to return date of today
	time_sec is the time you want to have in string*/
std::string		request::response::time_string(time_t time_sec) const //WARNING : time not allowed
{
	const int	max_size_of_tm = 5; /* seems be the maximum character of a time zone(tm)*/
	const int	str_size = 26 + max_size_of_tm + 1; /*	size of string will contain date, 26 for all character with day
														and month abreviated without length of time_zone and 1 for '\0' */ 
	struct tm	*time_tm; //date inside tm
	char		char_date[str_size] = {0}; //char_date will contain value of return before to be convert into a string

	/*WARNING : functions not allowed, we can maybe copy them with gettimeofday but it's seems boring to do it */
	time_tm = localtime(&time_sec); //set time_t

	/*	this function fill char_date with the information of date and with information of second string
		%a for day of week abreviated
		%d for day of month
		%b for month abreviated
		%Y for year
		%T for hour:minute:second
		%Z for time zone
	*/
	strftime(char_date, str_size, "%a, %d %b %Y %T %Z", time_tm);

	return (std::string(char_date));
}

/*Convert header_type to a syntax adapt for http do same thing than (std::string std::string::operator=(const header_type &)),
that doesn't exist for now, but it can be implement*/
std::string		request::response::header_to_string() const
{
	iterator it = header.begin();
	iterator end = header.end();
	std::string str;

	while (it != end)
	{
		str += it->first;
		str += ": ";
		str += it->second;
		str += "\n";
		++it;
	}
	return str;
}

/*add inside response:header all field which are in all method and in all condition*/
void			request::response::main_header(const std::string *allow_method)
{
	add_allow(allow_method); // allow_field add in header
	add_date(); //date field add in header
	add_server(); //server field add in header
}

std::string		request::response::header_first_line() const
{
	std::string		first_line;

	first_line = version + " " + ft_itoa(status) + " " + get_status_string() + "\n";

	return first_line;
}