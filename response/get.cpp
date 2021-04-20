#include <request.hpp>

/*this file regroup get function, get dont refer to the method but function to get a value through an other*/

/*return string associated to a status*/
std::string				request::response::get_status_string() const
{
	/*list of possible status, -1 serve as '\0' in a char *
	if you had element in this list you must add one in other and please respect the order*/
	const int			status_array[] = {200, 201, 204, 301, 304, 400, 401, 405, 406, 501, 503, -1};
	
	/*those string define in ../include/define.hpp are associated with one number of the other array to find what string use
	to what status if list grow use std::map can be judicious*/
	const std::string msg_status[] = {STATUS_200, STATUS_201, STATUS_204, STATUS_301, STATUS_304, STATUS_400,
									STATUS_401, STATUS_405, STATUS_406, STATUS_501, STATUS_503, UNKNOW_STATUS};

	int i = 0;

	while (status_array[i] != -1)
	{
		if (status == status_array[i])
			break;
		++i;
	}

	return msg_status[i];
}

/*this function return function associated to a method*/
request::response::response_function /*this line is the return type*/
request::response::get_method_function(const std::string &method, const std::string *allow_method) const
{
	/*function associated with array allow_method pass in parameter, it must be at least equal size, to compare array see
	public.cpp*/
	response_function	method_function[] = {&response::method_is_get, &response::method_is_head, &response::method_is_head,
											&response::method_is_head, &response::method_is_head, &response::method_is_head,
											&response::method_is_head, &response::method_is_head, &response::method_is_head,
											&response::method_is_head};
	size_t i;

	for(i = 0; allow_method[i] != ""; i++)
	{
		if (allow_method[i] == method)
			break;
	}
	return(method_function[i]);
}