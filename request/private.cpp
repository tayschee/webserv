#include <request.hpp>

/*	this function return a date into the form of string which is similar to : "Mon, 12 Mar 2088 12:27:32 GMT"
	this function have a default argument to return date of today
	time_sec is the time you want to have in string*/
std::string		request::today(time_t time_sec) const //WARNING : time not allowed
{
	const int	max_size_of_tm = 5; /* seems be the maximum character of a time zone(tm)*/
	const int	str_size = 26 + max_size_of_tm + 1; /*	size of string will contain date, 26 for all character with day
														and month abreviated without length of time_zone and 1 for '\0' */ 
	struct tm	*time_tm; //date inside tm
	char		char_date[str_size] = {0}; //char_date will contain value of return before to be convert into a string

	/*WARNING : functions not allowed, we can maybe copy them with gettimeofday but it's seems boring to do it */
	time_tm = localtime(&time_sec); //set time_t

	/*	this function fill char_date with the information of date with information of second string
		%a for day of week abreviated
		%d for day of month
		%b for month abreviated
		%Y for year
		%T for hour:minute:second
		%Z for time zone
	*/
	strftime(char_date, str_size, "%a, %d %b %Y, %T %Z", time_tm);
	
	return (std::string(char_date));
}