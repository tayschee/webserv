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

int				is_open(const struct stat &file)
{
	// IRWXU:  printf("le propriétaire a le droit de lecture\n");
	// IWUSR:  printf("le propriétaire a le droit d'écriture\n"); 
	// IXUSR:  printf("le propriétaire a le droit d'exécution\n");
	// IRWXG:  printf("lecture/écriture/exécution du groupe\n");
	// IRGRP:  printf("le groupe a le droit de lecture\n");
	// IWGRP:  printf("le groupe a le droit d'écriture\n");      
	// IXGRP:  printf("le groupe a le droit d'exécution\n");   
	// IRWXO:  printf("lecture/écriture/exécution des autres\n");   
	// IROTH:  printf("les autres ont le droit de lecture\n");   
	// IWOTH:  printf("les autres ont le droit d'écriture\n");   
	// IXOTH:  printf("les autres ont le droit d'exécution\n");
	if (!(file.st_mode & S_IRUSR)) // check read
		return (403);
	// if (!(file.st_mode & S_IWUSR)) // check write
	// 	return (403);
	// if (!(file.st_mode & S_IXUSR)) // check execution
	// 	return (403);
	return 0;
}

std::string		index(const std::string &path, std::string root, std::string add)
{
	DIR *dir = opendir(path.c_str());
	struct dirent *dp;
	std::string index =\
	"<html>\n\
	<head><title>Index of " + root + "</title></head>\n\
	<body bgcolor=\"white\">\n\
		<h1>Index of " + root + "</h1>\n\
		<hr><pre><a href=\"../\">../</a>\n";
		while ((dp = readdir(dir)) != NULL)
		{
			if (dp->d_name != std::string(".") && dp->d_name != std::string(".."))
				index += "<a href=\"" + add + std::string(dp->d_name) + "\">" + std::string(dp->d_name) + "/" + "</a>\n";
		}
        closedir(dir);
	index +=\
	"</pre><hr></body>\n\
	</html>";
	return index;
}

bool	is_authorize(const request &req, const parser &pars)
{
	parser::entries path(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
	if (path.find(AUTH_BASIC) != path.end())
	{
		message::header_type gh = req.get_header();
		if (gh.find(AUTHORIZATION) == gh.end())
			return false;
		std::vector<std::string> tab;
		std::string Authorization(req.get_header().find(AUTHORIZATION)->second);
		char buf[4096];
		int fd = open(path.find(AUTH_BASIC_USER_FILE)->second[0].c_str(), O_RDONLY);
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

	std::string file = find_path(pars.get_block("location", req.get_uri()), req);

	//403 interdiction
	if (file.empty())
		return 404;
	if (lstat(file.c_str(), &file_stat) < 0)
		return (error_file(errno));
	if (int ret = is_open(file_stat))
		return ret;
	if (!is_authorize(req, pars))
		return 401;
	//file = pars.get_block("server").conf.find("root")->second[0] + req.get_uri();
	std::cout << "path = " << file << std::endl;
	if (get_extension(file) == ".jpg")
	{
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
	if (get_extension(file) == ".php")
	{
		cgi(req, pars, body);
		if (body[0] == '5')
			return ft_atoi<int>(body);
	}
	else if ((file_stat.st_mode & S_IFMT) == S_IFDIR || (file_stat.st_mode & S_IFMT) == S_IFLNK)
	{
		std::string add = (file.substr(pars.get_block("location", req.get_uri()).conf.find("root")->second[0].size()));
		body = index(file, req.get_uri(), add);
	}
	else if (get_extension(file) == ".html")
	{	
		fd = open(file.c_str(), O_RDONLY);
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