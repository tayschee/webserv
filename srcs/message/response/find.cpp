#include <message/response.hpp>

/*this file regroup get function, get dont refer to the method but function to get a value through an other*/

/*return string associated to a status*/
response::status_array::value_type::second_type
response::find_status_string() const
{
	return existing_status.find(first_line.status)->second;
}

/*this function return function associated to a method*/
response::method_array::value_type::second_type /*same than method_function*/
response::find_method_function(const std::string &method, const std::vector<std::string> &allow_method) const
{
	std::vector<std::string>::const_iterator it(allow_method.begin());
	std::vector<std::string>::const_iterator end(allow_method.end());

	while (it < end)
	{
		if (method == *it)
			return existing_method.find(method)->second;
	}

	return existing_method.end()->second;
}

response::media_type_array::value_type
response::find_media_type(const std::string subtype) const
{
	const_iterator	val(existing_media_type.find(subtype));

	/*if subtype doesn't exist return default value*/
	if (val == existing_media_type.end())
		return (value_type(DEFAULT_SUBTYPE, DEFAULT_TYPE));

	return (*val);
}