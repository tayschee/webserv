#include "message/response.hpp"

response::response(const request &req, const parser::address_conf &pars_list, int &Pfdbody, int &Pfdin, int &Pfdout)
: message(), fdbody(Pfdbody), fdin(Pfdin), fdout(Pfdout)
{
	first_line.status = 400;
	const parser::address_conf::const_iterator pars_it = find_parser(pars_list, req);
	if (pars_it == pars_list.end() || (first_line.status = req.validity()) != 200)
	{
		main_header();
		first_line.status = error_response(first_line.status, req, *pars_it);
	}
	else
	{
		save_pars = &(*pars_it);
		const parser &pars(*pars_it);
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(PARSER_ACCEPT)->second); //no protect
		method_function method;
		method = find_method_function(req, allow_method, pars); //give function associate with request
		
		main_header(); /*add header_field which are present in all method*/

		first_line.status = generate_response(path_info, pars, req, method);
	}
	end_header(req);
}

response::response(int status, const request &req, const parser &pars, int &Pfdbody, int &Pfdin, int &Pfdout)
: message(), fdbody(Pfdbody), fdin(Pfdin), fdout(Pfdout)
{
	first_line.status = error_response(status, req, pars);
	end_header(req);
}

response& response::operator=(const response &other)
{
	first_line = other.first_line;
	func = other.func;
	save_path = other.save_path;
	body = other.body;
	header = other.header;
	return *this;
}

response::~response(){};