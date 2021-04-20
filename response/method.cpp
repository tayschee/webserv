#include "request.hpp"

int		request::response::method_is_head(const std::string &file, const header_type &req_head)
{
	struct stat file_stat; //information about file

	if (lstat(file.c_str(), &file_stat) < 0)
		return (100); //ERROR

	/*two next line can maybe be add to main_header*/
	add_content_length(req_head, file_stat.st_size); /*st_size = total size in byte*/
	add_last_modified(file_stat.st_mtime); /*st_mtime = hour of last modification*/

	return 200; //value of OK response
}

int		request::response::method_is_get(const std::string &file, const header_type &req_head)
{
	int		fd;
	struct stat file_stat; //information about file

	if ((fd = open(file.c_str(), O_RDONLY)) < 0)
		return 100; //ERROR
	if (lstat(file.c_str(), &file_stat) < 0)
		return (100); //ERROR

	add_body(fd, file_stat); /*body is filled by content of fd*/
	close(fd);

	//almost same than method_is_head but we dont call it because we need struct stat to fill body*/
	add_content_length(req_head, file_stat.st_size); /*st_size = total size in byte*/
	add_last_modified(file_stat.st_mtime); /*st_mtime = hour of last modification*/

	return 200;
}