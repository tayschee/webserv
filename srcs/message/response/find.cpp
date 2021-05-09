#include <message/response.hpp>

/*this file regroup get function, get dont refer to the method but function to get a value through an other*/

/*return string associated to a status*/
response::status_array::value_type::second_type //std::string
response::find_status_string() const
{
	status_array::const_iterator it(existing_status.find(first_line.status));
	
	if (it == existing_status.end())
		return UNKNOW_STATUS;
	else
		return existing_status.find(first_line.status)->second;
}

/*this function return function associated to a method*/
response::method_array::value_type::second_type //method_function
response::find_method_function(const std::string &method, const std::vector<std::string> &allow_method) const
{
	std::vector<std::string>::const_iterator it(allow_method.begin());
	std::vector<std::string>::const_iterator end(allow_method.end());

	while (it < end)
	{
		if (method == *it)
			return existing_method.find(method)->second;
	}
	return &response::method_is_unknow;
}

response::media_type_array::value_type //std::pair<std::string, std::string>
response::find_media_type(const std::string subtype) const
{
	media_type_array::const_iterator	val(existing_media_type.find(subtype));

	/*if subtype doesn't exist return default value*/
	if (val == existing_media_type.end())
		return (media_type_array::value_type(DEFAULT_SUBTYPE, DEFAULT_TYPE));

	return (*val);
}