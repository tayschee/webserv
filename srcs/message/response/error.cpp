#include "message/response.hpp"

/*after fail open() or fail stat pass to this function to determinate server error*/
int		response::error_file(int errnum) const
{
	/* maybe add this function to get* but to associate errno value to an error */

	//no authorisation || too much symbolic link (loop symlink) || file was directory
	if (errnum == EACCES || errnum == ELOOP || errnum == EISDIR)
		return 403; //Forbidden
	else if (errnum == ENOENT || errnum == ENOTDIR) //file doesnt exist || a element of path is not a directory (except last)
		return 404; //Not Found
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

void	response::default_error(int error_status, const request &req)
{
	size_t pos(0);
	size_t size_str_to_replace(ft_strlen(STR_TO_REPLACE));
	std::string new_str(ft_itoa(error_status) + " " + find_status_string());
	size_t size_new_str(new_str.size());
	body = DEFAULT_ERROR_FILE;

	while ((pos = body.find(STR_TO_REPLACE, pos)) != body.npos)
	{
		body.replace(pos, size_str_to_replace, new_str);
		pos += size_new_str;
	}
	add_content_length(body.size());
	add_content_type(DEFAULT_ERROR_FILE_EXT);

	/*do something else if there is particular thing for all method*/
	if (req.get_method() == HEAD)
		body.clear();
}

int response::error_msg(const std::string &path, const request &req, const parser &pars)
{
	int status;

	status = method_is_get(path, req, pars);
	std::cout << "status : " << status << "\n";
	if (first_line.status == 404 && status == 404)
	{
		std::cout << "nive\n";
		default_error(status, req);
		return 200;
	}
	else if (status > 299)
		return status;
	else
		return 200;
}

int	response::error_response(int status, const request &req, const parser &pars)
{
	std::cout << "abc\n";
	int status_error(status);
	std::cout << "ded\n";
	std::map<int, std::string> block = pars.get_block(PARSER_SERVER).errors;
	std::cout << "kjl\n";
	std::map<int, std::string>::iterator it;
	std::cout << "xxx\n";
	std::map<int, std::string>::const_iterator end(block.end());
	std::cout << "yyy\n";

	while (status > 299)
	{
		status_error = status;
		std::cout << "HERE !!!!!\n";
		it = block.find(status);
		if (it == end)
		{
			default_error(status, req);
			break;
		}
		else
		{
			status = error_msg(it->second, req, pars);
		}
	}
	status_header();
	error_special_case(req); //delete things which are note in specific method

	return status_error;
}

int	response::error_response(int status, const request &req)
{
	default_error(status, req);
	status_header();
	error_special_case(req); //delete things which are note in specific method

	return status;
}