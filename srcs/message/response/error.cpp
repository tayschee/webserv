#include "message/response.hpp"

/*after fail open() or fail stat pass to this function to determinate server error*/
int response::error_file(int errnum) const
{
	/* maybe add this function to get* but to associate errno value to an error */

	//no authorisation || too much symbolic link (loop symlink) || file was directory
	if (errnum == EACCES || errnum == ELOOP || errnum == EISDIR)
		return 403;									//Forbidden
	else if (errnum == ENOENT || errnum == ENOTDIR) //file doesnt exist || a element of path is not a directory (except last)
		return 404;									//Not Found
	else
		return 500; //server error ?
}

void	response::error_special_case(const request &req)
{
	if (req.get_method() == HEAD)
		body.clear();
	if (req.get_method() == CONNECT)
	{
		body.clear();
		header.erase(CONTENT_LENGTH);
	}
}

void	response::default_error(int error_status, const request &req, const parser &pars)
{
	size_t pos(0);
	size_t size_str_to_replace(ft_strlen(STR_TO_REPLACE));
	std::string new_str(ft_itoa(error_status) + " " + find_status_string(error_status));
	size_t size_new_str(new_str.size());
	body = DEFAULT_ERROR_FILE;

	while ((pos = body.find(STR_TO_REPLACE, pos)) != body.npos)
	{
		body.replace(pos, size_str_to_replace, new_str);
		pos += size_new_str;
	}
	add_content_length(body.size());
	add_content_type(find_media_type(DEFAULT_ERROR_FILE_EXT, pars));

	/*do something else if there is particular thing for all method*/
	if (req.get_method() == HEAD)
		body.clear();
}

int response::redirect_to_error(const std::string &path, const request &req, const parser &pars)
{
	int status;
	method_function method;

	std::cout << "PATH DANS REDIRECT_TO_ERROR = " << path << std::endl;
	if (is_cgi(get_extension(path), pars, req.get_method()))
	{
	std::cout << "method = post" << std::endl;

			method = existing_method.find(POST)->second;
	}
	else
		method = req.get_method() == HEAD ? &response::method_is_head : &response::method_is_get;

	std::cout << "METHOD FINI" << "\n";
	parser::entries path_info(pars.get_block(PARSER_LOCATION, path).conf);
	status = (this->*method)(path, req, pars); //change for if there is redirect


	return status;
}

int response::error_response(int status, const request &req, const parser &pars)
{
	std::map<int, std::string> block = pars.get_block(PARSER_SERVER).errors;
	std::map<int, std::string>::iterator it;
	std::map<int, std::string>::const_iterator end(block.end());

	it = block.find(status);
	std::cout << "status2 : " << status << "\n";
	if (it == end)
	{
		std::cout << "status : " << status << "\n";
		default_error(status, req, pars);
		std::cout << "body : " << body << "\n";
	}
	else
	{
		std::cout << "stop here\n";
		if (redirect_to_error(it->second, req, pars) == 404)
		{
			std::cout << "failed\n";
			default_error(status, req, pars);
		}
	}
	std::cout << "==========================" << std::endl;
	status_header(status, req.get_uri(), pars);
	std::cout << "==========================2" << std::endl;
	error_special_case(req); //delete things which are note in specific method
	std::cout << "==========================3" << std::endl;
	//std::cout << "header : " << get_header() << "\n";
	//std::cout << "body : " << get_body() << "\n";
	//std::cout << "test : " << header.find(CONTENT_LENGTH)->second << "\n"; 
	//std::cout << "test : " << header.find(WWW_AUTHENTICATE)->second << "\n"; 

	return status;
}

int response::error_response(int status, const request &req)
{
	std::cout << "status1 : " << status << "\n";
	//default_error(status, req);
	//status_header(status, req.get_uri(), path);
	error_special_case(req); //delete things which are note in specific method

	return status;
}
