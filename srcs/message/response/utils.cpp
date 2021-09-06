#include "message/response.hpp"

/*Convert header_type to a syntax adapt for http do same thing than (std::string std::string::operator=(const header_type &)),
that doesn't exist for now, but it can be implement*/
std::string response::header_to_string() const
{
	const_iterator it(header.begin());
	const_iterator end(header.end());
	std::string str;

	while (it != end)
	{
		str += it->first;
		str += ": ";
		str += it->second;
		str += CRLF;
		++it;
	}
	return str;
}

/*add inside response:header all field which are in all method and in all condition*/
void response::main_header(const std::vector<std::string> &allow_method)
{
	add_allow(allow_method); // allow_field add in header
	add_date();				 // date field add in header
	add_server();			 // server field add in header
}

/*add inside response:header all field which are in all method and in all condition without allow use for error constructor*/
void response::main_header()
{
	add_date();	  //date field add in header
	add_server(); //server field add in header
}

/*create first line of response header */
std::string response::header_first_line() const
{
	std::string str_first_line;

	str_first_line = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.version + CRLF;

	return str_first_line;
}

// Check authorizations
int	response::is_authorize(const std::string &uri, const request &req, const parser &pars) const
{
	parser::entries path(pars.get_block(BLOCK_LOCATION, uri).conf);
	if (path.find(AUTH_BASIC_USER_FILE) != path.end())
	{
		std::vector<std::string> tab;
		std::string				user, user2;
		struct stat file_stat; //information about file
		char buf[4096];
		int fd = 0;

		std::string file = path.find(AUTH_BASIC_USER_FILE)->second[0].c_str();
		if (lstat(file.c_str(), &file_stat) < 0)
			return 500;
		std::string Authorization(req.get_user());
		if (Authorization.empty())
			return 401;
		if ((fd = open(file.c_str(), O_RDONLY | O_CREAT, 0666)) < 0)
			return 403;
		int ret = 0;
		if ((ret = read(fd, buf, 4095)) < 0)
			return 403;
		if (!ret)
			return 0;
		buf[ret] = '\0';
		tab = split(buf, "\t\r\n");
		memset(buf, 0, 4096);
		close(fd);
		for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end(); ++it)
		{
			if (*it == Authorization)
				return 0;
		}
		return 401;
	}
	return 0;
}

void response::status_header(int status)
{
	if (status == 401)
	{
		std::cout << "www_autentificate\n";
		add_www_autentificate();
	}
	if (status == 503)
		add_retry_after(200);
	//if (first_line.status > 299 && first_line.status < 400)
	//	add_retry_after(1);
}

int			response::is_open(const struct stat &file) const
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

/*if there is accept-language header field delete extension of langauge*/
std::string		&response::file_without_language_ext(std::string &path) const
{
	header_type::const_iterator it = header.find(ACCEPT_CHARSET);
	header_type::const_iterator end = header.end();
	size_t pos;

	if (it == end)
		return path;

	pos = path.find_last_of("." + it->second);
	if (pos != path.npos)
		return path.erase(pos);
	else
		return path;

}

// Delete function is a recursive called by method_is_delete
int		response::del_content(std::string path, const request &req, const parser &pars, const bool del)
{
	struct stat file_stat; //information about file
	int ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;
	if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
	{
		DIR *dir = opendir(path.c_str());
		struct dirent *dp;
		while ((dp = readdir(dir)) != NULL)
		{
			int ret = 0;
			if (std::string(dp->d_name) != std::string(".") && std::string(dp->d_name) != std::string(".."))
			{
				if (path.begin() != path.end() && *(--path.end()) != '/')
					path.push_back('/');
				if ((ret = del_content(path + std::string(dp->d_name), req, pars, del)) != 0)
				{
					closedir(dir);
					return ret;
				}
			}
		}
		closedir(dir);
		if (del)
			rmdir(path.c_str());
	}
	else if (del && (ret = unlink(path.c_str())) != 0) //delete the file, if there is a fd associted whith this file, deleted it when the fd is close
	{
		return 403;
	}
	return 0;
}

// Check the path
int		response::check_path(const std::string &path, struct stat &file_stat, const request &req, const parser &pars) const
{
	(void)req;
	(void)pars;
	if (path.empty())
		return 404;
	if (lstat(path.c_str(), &file_stat) < 0)
		return 404;
	if (int ret = is_open(file_stat))
		return ret;
	// if (!is_authorize(path, req, pars))
	// 	return 401;
	return (0);
}

// Check if the type is a CGI
bool		response::is_cgi(const std::string &type, const parser &pars, const std::string &method) const
{
	try
	{
		parser::entries bc(pars.get_block("cgi", type).conf);
		std::vector<std::string> tab = bc.find("accept")->second;

		for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end(); ++it)
		{
			if (*it == method)
				return true;	
		}
		return false;
	}
	catch(const std::exception& e)
	{
		return false;
	}
	return true;
}

// Manage autoindex
std::string		response::index(const std::string &path, std::string root, std::string add) const
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

// Manage codes
/*void	response::get_code(const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	if (first_line.status == 401)
		header.insert(value_type(WWW_AUTHENTICATE, "Basic realm=\"Accès au site de webserv\", charset=\"UTF-8\""));
	if (first_line.status == 503)
		header.insert(value_type("Retry-after",  "20000"));

	char buff[PATH_MAX];
	getcwd( buff, PATH_MAX );
	std::string file_error = std::string(buff) + "/error/" + std::string(ft_itoa(first_line.status)) + ".html";
	//td::string file_error = getwd() + "/Users/jelarose/Documents/web/error/" + std::string(ft_itoa(first_line.status)) + ".html";
	if (lstat(file_error.c_str(), &file_stat) < 0)
	{
		file_error = std::string(buff) + "/error/404.html";
		lstat(file_error.c_str(), &file_stat);
	}
	int fd = open(file_error.c_str(), O_RDONLY);
	char buf[file_stat.st_size + 1];
	int ret = read(fd, buf, file_stat.st_size);
	buf[ret] = '\0';
	body = buf;
	memset(buf, 0, file_stat.st_size + 1);
	header.insert(value_type(CONTENT_LENGTH,  ft_itoa(body.size())));
	header.insert(value_type(CONTENT_TYPE,  "text/html"));
	close(fd);
}*/

// Manage redirections
/*bool		response::is_redirect(parser::entries &block, const parser &pars)
{
	std::string redirect;

	if (block.find("return") == block.end())
		return 0;
	redirect = block.find("return")->second[0];

	if (!redirect.empty())
	{

		first_line.status = ft_atoi<int>(redirect);
	//	header.insert(value_type(CONTENT_TYPE, "application/octet-stream"));

		std::string location = block.find("return")->second[1];

		if (first_line.status == 301 || first_line.status == 302 || first_line.status == 303
		|| first_line.status == 307 || first_line.status == 308)
		{
			header.insert(value_type(LOCATION, location));
			get_code(pars);
		}
		else
			body = location;
		return 1;
	}
	return 0;
}*/

// Manage redirections
int response::is_redirect(const parser::entries &block, const parser &pars, const request &req)
{
	(void)req;
	parser::entries::const_iterator return_line(block.find(PARSER_RETURN));
	std::string redirect;
	int status;

	(void)pars;
	if (return_line == block.end())
		return 0;

	std::vector<std::string>::const_iterator return_arg = block.find(PARSER_RETURN)->second.begin();

	header.insert(value_type(CONTENT_TYPE, "application/octet-stream")); //CHANGE
	status = ft_atoi<int>(*return_arg);
	header.insert(value_type(LOCATION, *++return_arg));

	return status;
}

int response::generate_response(const parser::entries &path_info, const parser &pars, const request &req, const method_function &method)
{
	int status;

	if (path_info.find(BODY_SIZE) != path_info.end() && req.get_body().size() > ft_atoi<unsigned long>(path_info.find("body_size_max")->second[0]))
	{
		status = 413;
	}
	else if (!(status = is_redirect(path_info, pars, req))) //do function with all condition
	{
		//call pointer to member function this is exactly like that we must call it, ALL bracket are neccessary there is no other way
		status = (this->*method)(req.get_uri(), req, pars);
	}
	else
	{
		std::cout << "not here\n";
	}
	if (status > 299)
	{
		std::cout << "here\n";
		status = error_response(status, req, pars);
		std::cout << "status : " << status << "\n";
	}
	return status;
}

std::string			response::header_in_order(const std::string &hf_sep, const std::string &eol, const std::vector<std::string> &list) const
{
	const_iterator	it;
	const_iterator	end(header.end());
	std::string resp_str;
	size_t i = 0;

	while (i < list.size())
	{
		it = header.find(list[i]);
		if (it != end)
		{
			resp_str += it->first + hf_sep + it->second + eol;
		}
		++i;
	}
	return resp_str;
}