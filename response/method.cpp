#include "request.hpp"

int		request::response::method_is_head(const std::string &file, const header_type &req_head, const std::string &body)
{
	struct stat file_stat; //information about file

	(void)body;

	if (lstat(file.c_str(), &file_stat) < 0)
		return (100); //ERROR

	/* two next line can maybe be add to main_header */
	add_content_length(req_head, file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200; //value of OK response
}

int		request::response::method_is_get(const std::string &file, const header_type &req_head, const std::string &body)
{
	int		fd;
	struct stat file_stat; //information about file

	(void)body;

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

int		request::response::method_is_delete(const std::string &file, const header_type &req_head, const std::string &body)
{
	int		fd;
	struct stat file_stat; //information about file
	std::string delete_msg(DELETE_FILE);

	(void)body;

	if ((fd = open(file.c_str(), O_RDWR, O_TRUNC)) < 0)
		return 100; //ERROR
	if (lstat(file.c_str(), &file_stat) < 0)
		return 100; //ERROR

	write(fd, delete_msg.c_str(), delete_msg.size()); //overwrite file with delete_msg
	add_body(fd, file_stat); /*body is filled by content of fd (delete_msg)*/
	close(fd);

	/* almost same than method_is_head but we dont call it because we need struct stat to fill body */
	add_content_length(req_head, file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file);

	return 200;
}

int		request::response::method_is_options(const std::string &file, const header_type &req_head, const std::string &body)
{
	return (method_is_head(file, req_head, body));
}

int		request::response::method_is_put(const std::string &file, const header_type &req_head, const std::string &body)
{
	int		fd;
	int		response_value = 200;

	if ((fd = open(file.c_str(), O_RDWR)) < 0)
	{
		response_value = 201; //CREATE
		if ((fd = open(file.c_str(), O_RDWR | O_CREAT)) < 0)
		{	
			return 100; //ERROR
		}
	}

	(void)req_head;
	write(fd, body.c_str(), body.size());
	close(fd);

	add_content_type(file);

	return response_value;
}