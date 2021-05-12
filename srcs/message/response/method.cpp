#include "message/response.hpp"

int		response::method_is_head(const request &req, const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (lstat(file.c_str(), &file_stat) < 0)
	{
		return (error_file(errno));
	}

	/* two next line can maybe be add to main_header */
	add_content_length(file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file, req);

	return 200; //value of OK response
}

std::string		index(std::string path)
{
    errno = 0;
	DIR *dir = opendir(path.c_str());\
	struct dirent *dp;
	std::string index =\
	"<html>\n\
	<head><title>Index of /</title></head>\n\
	<body>\n\
		<h2>Index of /</h2>\n\
		<ui>\n\
		<ur>";\
			while ((dp = readdir(dir)) != NULL)
			{
				index += "<li><a href=\"" + std::string(dp->d_name) + "\">" + std::string(dp->d_name) + "</a></li>\n";
			}
			if (errno != 0)
				return "500";
            closedir(dir);

	index +=\
		"<ui>\n\
	</body>\n\
	</html>";
	return index;
}

bool	is_authorize(const request &req, const parser &pars)
{
	parser::entries path = pars.get_block("location", "/").conf;
	if (path.find("auth_basic") != path.end())
	{
		message::header_type gh = req.get_header();
		if (gh.find("Authorization") == gh.end())
			return false;
		std::vector<std::string> tab;
		std::string Authorization(req.get_header().find("Authorization")->second);
		char buf[500];
		int fd = open(path.find("auth_basic_user_file")->second[0].c_str(), O_RDONLY);
		int ret = read(fd, buf, 499);
		buf[ret] = '\0';
		tab = split(buf, "\t\r\n");
		close(fd);
		for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end(); ++it)
			if (Authorization == *it)
				return true;
		return false;
	}
	return true;
}

int		response::method_is_get(const request &req, const parser &pars)
{
	int		fd = -1;
	struct stat file_stat; //information about file

	std::string	file = pars.get_block("server").conf.find("root")->second[0] + req.get_uri();

	//403 interdiction
	
	if (lstat(file.c_str(), &file_stat) < 0)
		return (error_file(errno));
	std::string CGI(req.get_uri());
	std::string::iterator it = CGI.end();
	while (--it != CGI.begin())
	{
		if (*it == '.')
			break;
	}
	std::string cmp(it, CGI.end());
	if (cmp == ".jpg")
	{
		if (!is_authorize(req, pars))
			return 401;
		fd = open(file.c_str(), O_RDONLY);
		if (add_body(fd, file_stat)) /*body is filled by content of fd*/
		{
			close(fd);
			return error_file(errno);
		}
		add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
		header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size())));
		header.insert(value_type(CONTENT_TYPE, "text/jpeg"));
		return 200;
	}
	if (cmp == ".php")
	{
		if (!is_authorize(req, pars))
			return 401;
		cgi(req, pars, body);
		if (body[0] == '5')
			return ft_atoi<int>(body);
	}
	else if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
	{
		body = index(file);
	}
	else 
	{
		if (!is_authorize(req, pars))
			return 401;
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
		/* almost same than method_is_head but we dont call it because we need struct stat to fill body */
		//add_content_length(req.get_header(), file_stat.st_size); /* st_size = total size in byte */
		header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size())));

		add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
		add_content_type(file, req);
		if (cmp == ".jpg")
			header.insert(value_type(CONTENT_TYPE, "image/jpeg"));

	}
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	//add_content_type(file);
	header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size())));
	header.insert(value_type(CONTENT_TYPE, "text/html"));
	if (fd > 0)
		close(fd);
	return 200;
}
/*
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

	if (add_body(fd, file_stat)) //body is filled by content of fd//
	{
		close(fd);
		return error_file(errno);
	}
	close(fd);

	// almost same than method_is_head but we dont call it because we need struct stat to fill body //
	add_content_length(req.get_header(), file_stat.st_size); // st_size = total size in byte //
	add_last_modified(file_stat.st_mtime); // st_mtime = hour of last modification //
	add_content_type(file);

	return 200;
}*/

int		response::method_is_delete(const request &req, const parser &pars)
{
	(void)pars;
	std::string	file = req.get_uri();

	//check if path is valid

	if (unlink(file.c_str())) //delete the file, if there is a fd associted whith this file, deleted it when the fd is close
	{
		return error_file(errno); //check errno
	}

	return 204;
}

int		response::method_is_options(const request &req, const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (stat(file.c_str(), &file_stat) < 0)
		return error_file(errno); //check errno

	/* two next line can maybe be add to main_header */
	add_content_length(0); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file, req);

	return 200; //value of OK response
}

int		response::method_is_put(const request &req, const parser &pars)
{
	(void)pars;
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
		add_content_length(0); //Content-length is for size of body and there is no body
		//must add location
	}
	if (write(fd, body.c_str(), body.size()) < 0)
		return 500;
	close(fd);

	add_content_type(file, req);

	return response_value;
}

int			response::method_is_unknow(const request &req, const parser &pars)
{
	(void)pars;
	(void)req;

	return 405; //Method Not Allowed
}