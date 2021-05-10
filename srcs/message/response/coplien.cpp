#include <message/response.hpp>
#include <iostream>

void	response::get_error(int error, const parser &pars)
{
	struct stat file_stat; //information about file

	std::string file_error = pars.get_block("location_error", "40x.html").conf.find("root")->second[0] +
	"/" + std::string(ft_itoa(error)) + ".html";
	if (lstat(file_error.c_str(), &file_stat) < 0)
	{
		file_error = pars.get_block("location_error", "40x.html").conf.find("root")->second[0] + "/404.html";
		lstat(file_error.c_str(), &file_stat);
	}
	int fd = open(file_error.c_str(), O_RDONLY);
	char buf[file_stat.st_size + 1];
	read(fd, buf, file_stat.st_size);
	buf[file_stat.st_size] = '\0';
	body = buf;
	header["Content-Length"] =  ft_itoa(body.size());
	header["Content-Type"] =  "text/html";
}

response::response(const request &req, const parser &pars)
{
	std::string allow_method[] = {GET, HEAD, PUT, POST, CONNECT, OPTIONS, DELETE, PATCH, ""}; //will be replaced
	/*without typedef method_function f write it, typedef int (response::*f)(const request &req). this is pointer to function*/
	method_function header_field_function = find_method_function(req.get_method(), allow_method); //give function associate with request
	
	main_header(allow_method); /*add header_field which are present in all method*/
	/*call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way*/
	
	first_line.status = (this->*header_field_function)(req, pars);
	if (first_line.status == -1)
		return;
	else if (first_line.status < 200 || first_line.status > 299)
		get_error(first_line.status, pars);
	else
		first_line.status_string = find_status_string();

	first_line.version = req.get_version();

}

response::~response(){};

