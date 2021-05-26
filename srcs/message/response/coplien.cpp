#include "message/response.hpp"
#include <iostream>

response::response(const request &req, const parser &pars) : message()
{
	if (req.validity() != 0)
	{
		first_line.status = 404;
		get_code(pars);
		first_line.status_string = find_status_string();
	}
	else
	{
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		
		std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
		/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/

		method_function header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request

		main_header(allow_method); /*add header_field which are present in all method*/
		/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
		if (!is_redirect(path_info, pars))
		{

			first_line.status = (this->*header_field_function)(req, pars);

			if (first_line.status < 200 || first_line.status > 299)
				get_code(pars);
		}
		
		first_line.status_string = find_status_string();
		first_line.version = req.get_version();
	}
}

/*response::response(int status, const parser &pars) : message()
{
	first_line.status = status;
	first_line.status_string = find_status_string();
	first_line.version = HTTP_VERSION;

	//parser::entries path_info(pars.get_block(BLOCK_SERVER).conf);
	//std::vector<std::string> (path_info.find(ACCEPT)->second);
	(void)pars;

	//fonction pour trouver les pages d'erreurs
	//si il y en a une
		//mettre un fonction
	//sinon
		default_error(status);
}*/

response::~response(){};

