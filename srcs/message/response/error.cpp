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

void	response::default_error(int error_status)
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
}