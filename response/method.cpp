#include "message/response.hpp"

int		response::method_is_head(const request &req)
{
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (lstat(file.c_str(), &file_stat) < 0)
	{
		return (error_file(errno));
	}

	/* two next line can maybe be add to main_header */
	add_content_length(req.get_header(), file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200; //value of OK response
}

int		response::method_is_get(const request &req)
{
	int		fd;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	//403 interdiction
	if ((fd = open(file.c_str(), O_RDONLY)) < 0)
	{
		return (error_file(errno));
	}
	if (lstat(file.c_str(), &file_stat) < 0)
		return (error_file(errno));

	if (add_body(fd, file_stat)) /*body is filled by content of fd*/
	{
		close(fd);
		return error_file(errno);
	}
	close(fd);

	/* almost same than method_is_head but we dont call it because we need struct stat to fill body */
	add_content_length(req.get_header(), file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200;
}

int		response::method_is_delete(const request &req)
{
	std::string	file = req.get_uri();

	//check if path is valid

	if (unlink(file.c_str())) //delete the file, if there is a fd associted whith this file, deleted it when the fd is close
	{
		return error_file(errno); //check errno
	}

	return 204;
}

int		response::method_is_options(const request &req)
{
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (lstat(file.c_str(), &file_stat) < 0)
		return error_file(errno); //check errno

	/* two next line can maybe be add to main_header */
	add_content_length(req.get_header(), 0); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200; //value of OK response
}

int		response::method_is_put(const request &req)
{
	int		fd;
	int		response_value = 204;
	std::string	file = req.get_uri();
	std::string body = req.get_body();

	/*verify if content exist*/
	if ((fd = open(file.c_str(), O_WRONLY)) < 0)
	{
		response_value = 201; //CREATE
		if ((fd = open(file.c_str(), O_WRONLY | O_CREAT)) < 0) //content doesn't exist so create it
		{	
			return (error_file(errno));
		}
		/* this header field are specific if file didn't exists */
		add_content_length(req.get_header(), 0); //Content-length is for size of body and there is no body
		//must add location
	}
	if (write(fd, body.c_str(), body.size()) < 0)
		return 500;
	close(fd);

	add_content_type(file);

	return response_value;
}

int			response::method_is_unknow(const request &req)
{
	(void)req;

	return 405; //Method Not Allowed
}