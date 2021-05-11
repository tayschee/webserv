#include <message/response.hpp>

const response::method_array response::existing_method(response::initialise_existing_method());

response::method_array	response::initialise_existing_method()
{
	const size_t			size(9);
	const std::string		method_name[size] = {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};
	const method_function	method_func[size] = {&response::method_is_get, &response::method_is_head, &response::method_is_unknow,
											 &response::method_is_put, &response::method_is_delete, &response::method_is_unknow,
											 &response::method_is_unknow, &response::method_is_options, &response::method_is_unknow};

	method_array	map;
	size_t			i;

	for(i = 0; i < size; i++)
	{
		map.insert(method_array::value_type(method_name[i], method_func[i]));
	}

	return map;
}

const response::media_type_array		response::existing_media_type(response::initialise_existing_media_type());

response::media_type_array	response::initialise_existing_media_type()
{
	media_type_array	map;

	//APPLICATION
	map.insert(media_type_array::value_type(BMP, APP));
	map.insert(media_type_array::value_type(GIF, APP));
	map.insert(media_type_array::value_type(JPEG, APP));
	map.insert(media_type_array::value_type(PNG, APP));
	map.insert(media_type_array::value_type(WEBP, APP));

	//AUDIO
	map.insert(media_type_array::value_type(BMP, AUDIO));
	map.insert(media_type_array::value_type(GIF, AUDIO));
	map.insert(media_type_array::value_type(JPEG, AUDIO));
	map.insert(media_type_array::value_type(PNG, AUDIO));
	map.insert(media_type_array::value_type(WEBP, AUDIO));

	//IMAGE
	map.insert(media_type_array::value_type(BMP, IMAGE));
	map.insert(media_type_array::value_type(GIF, IMAGE));
	map.insert(media_type_array::value_type(JPEG, IMAGE));
	map.insert(media_type_array::value_type(PNG, IMAGE));
	map.insert(media_type_array::value_type(WEBP, IMAGE));

	//TXT
	map.insert(media_type_array::value_type(CSS, TXT));
	map.insert(media_type_array::value_type(HTML, TXT));
	map.insert(media_type_array::value_type(JS, TXT));

	//VIDEO
	map.insert(media_type_array::value_type(BMP, VIDEO));
	map.insert(media_type_array::value_type(GIF, VIDEO));
	map.insert(media_type_array::value_type(JPEG, VIDEO));
	map.insert(media_type_array::value_type(PNG, VIDEO));
	map.insert(media_type_array::value_type(WEBP, VIDEO));

	//inside function tou must verify if map == end()

	return map;
}

const  response::status_array	response::existing_status(response::initialise_existing_status());

response::status_array			response::initialise_existing_status()
{
	const size_t			size(37); //size of two array
	const int		 status_int[size] = {200, 201, 202, 203, 204, 205, 206,
									300, 301, 302, 303, 304, 305, 307,
									400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 417, 426,
									501, 502, 503, 504, 505}; //key array
	const std::string msg_status[size] = {STATUS_200, STATUS_201, STATUS_202, STATUS_203, STATUS_204, STATUS_204, STATUS_205,
										STATUS_301, STATUS_302, STATUS_303, STATUS_304, STATUS_305, STATUS_307,
										STATUS_400, STATUS_401, STATUS_402, STATUS_403, STATUS_404, STATUS_405, STATUS_406, STATUS_407,
										STATUS_408, STATUS_409, STATUS_410, STATUS_411, STATUS_412, STATUS_413, STATUS_414, STATUS_415, STATUS_417, STATUS_426,
										STATUS_501, STATUS_502, STATUS_503, STATUS_504, STATUS_505}; //value_array

	status_array	map;
	size_t			i;

	for(i = 0; i < size; i++)
	{
		map.insert(status_array::value_type(status_int[i], msg_status[i]));
	}

	return map;
}
