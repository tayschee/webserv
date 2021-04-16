#include "request.hpp"

int		request::response::method_is_head(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	struct stat file_stat; //information about file

	(void)req_body;

	if (lstat(file.c_str(), &file_stat) < 0)
		return (100); //ERROR

	/* two next line can maybe be add to main_header */
	add_content_length(req_head, file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200; //value of OK response
}

int		request::response::method_is_get(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	int		fd;
	struct stat file_stat; //information about file

	(void)req_body;

	if ((fd = open(file.c_str(), O_RDONLY)) < 0)
		return 100; //ERROR
	if (lstat(file.c_str(), &file_stat) < 0)
		return 100; //ERROR

	add_body(fd, file_stat); /*body is filled by content of fd*/
	close(fd);

	/* almost same than method_is_head but we dont call it because we need struct stat to fill body */
	add_content_length(req_head, file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200;
}

int		request::response::method_is_delete(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	(void)req_body;
	(void)req_head;

	//check if path is valid

	if (unlink(file.c_str())) //delete the file, if there is a fd on this file, deleted it when the fd is close
		return 100; //check errno

	return 204;
}

int		request::response::method_is_options(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	struct stat file_stat; //information about file

	(void)req_body;

	if (lstat(file.c_str(), &file_stat) < 0)
		return (100); //ERROR

	/* two next line can maybe be add to main_header */
	add_content_length(req_head, 0); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200; //value of OK response
}

int		request::response::method_is_put(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	int		fd;
	int		response_value = 204;

	/*verify if content exist, if not, create it*/
	if ((fd = open(file.c_str(), O_WRONLY)) < 0)
	{
		response_value = 201; //CREATE
		if ((fd = open(file.c_str(), O_WRONLY | O_CREAT)) < 0)
		{	
			return 100; //ERROR
		}
		/* this header field are specific if file didn't exists */
		add_content_length(req_head, 0); //Content-length is for size of body and there is no body
		//must add location
	}
	write(fd, req_body.c_str(), req_body.size());
	close(fd);

	add_content_type(file);

	return response_value;
}

int			request::response::method_is_unknow(const std::string &file, const header_type &req_head, const std::string &req_body)
{
	(void)file;
	(void)req_head;
	(void)req_body;

	return 405;
}