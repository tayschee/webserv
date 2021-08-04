# include "message/request.hpp"

const request::method_array request::existing_method(request::initialise_existing_method());

request::method_array request::initialise_existing_method()
{
	int						i(0);
	const int				size(9); 
	const std::string		string_array[size] = {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};
	method_array			array; //j'ai pas reussi a appeler le constructeur first, end

	array.reserve(9);
	for (i = 0; i < 9; ++i)
	{
		array.push_back(string_array[i]);
	}

	return array;
}