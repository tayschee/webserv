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
		errno = 0;
		std::string Authorization(req.get_user());

		if (Authorization.empty() || Authorization == "Og==")
			return 401;
		std::string file = path.find(AUTH_BASIC_USER_FILE)->second[0].c_str();
		if (lstat(file.c_str(), &file_stat) < 0)
				return 403;

		if ((fd = open(file.c_str(), O_RDONLY | O_CREAT, 0666)) < 0)
			return 500;
		int ret = 0;
		if ((ret = read(fd, buf, 4095)) < 0)
			return 500;
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

void			response::status_header(int status, const std::string &path, const parser &pars)
{
	if (status == 401)
	{
		add_www_autentificate(pars, path);
	}
	if (status == 503)
		add_retry_after(200);
	//if (first_line.status > 299 && first_line.status < 400)
	//	add_retry_after(1);
}

bool			response::is_acces(const struct stat &file) const
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
	errno = 0;

	if (lstat(path.c_str(), &file_stat) < 0)
	{
		if (errno == ENOENT)
			return 404;
		if (errno == EACCES)
			return 403;
	}
	 if (is_acces(file_stat))
	 	return 403;
	return (0);
}

// Check if the type is a CGI
bool		response::is_cgi(const std::string &type, const parser &pars, const std::string &method) const
{
	try
	{
		parser::entries bc(pars.get_block(BLOCK_CGI, type).conf);
		std::vector<std::string> tab;
		
		if (bc.find(ACCEPT) != bc.end())
			tab = bc.find(ACCEPT)->second;
		else
			tab = pars.get_block(BLOCK_SERVER).conf.find(ACCEPT)->second;

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
			{
				struct stat file_stat;
				stat( (path + dp->d_name).c_str(), &file_stat);
				index += "<a href=\"" + add + std::string(dp->d_name);
				if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
					index += "/\">" + std::string(dp->d_name) + "/";
				else
					index += "\">" + std::string(dp->d_name);
				index += "</a>\n";
			}
		}
        closedir(dir);
	index +=\
	"</pre><hr></body>\n\
	</html>";
	return index;
}

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

	header.insert(value_type(CONTENT_TYPE, APP_OCT_STREAM)); //CHANGE
	status = ft_atoi<int>(*return_arg);
	std::string host = req.get_host();
	
	if (host.find("http") == host.npos)
		host = "http://" + host;

	size_t i = -1;
	int v = 0;
	while (++i < host.size())
	{
		if (host[i] == ':')
		{
			if (++v == 2)
			{
				host = host.substr(0, i);	
				break;
			}
		}
	}
	header.insert(value_type(LOCATION, host += *++return_arg));

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
	if (status > 299)
		status = error_response(status, req, pars);
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

int		response::sent(int fd, request &req, const std::string &hf_sep, const std::string &eol)
{
	std::string resp_str;

	if (req.get_method() == HEAD)
	{
		if (header.find(CONTENT_LENGTH) != header.end() && header.find(CONTENT_LENGTH)->second == "0")
			header.erase(CONTENT_LENGTH);
		if (first_line.status >= 300 && header.find(LAST_MODIFIED) != header.end())
		{
			header.erase(LAST_MODIFIED);
		}
		if (first_line.status >= 500)
		{
			header.insert(value_type(CONNECTION, CLOSE));
			req.set_connexion(CLOSE);
		}
		else if (first_line.status >= 200)
			header.insert(value_type(CONNECTION, req.get_connexion()));
	}

	resp_str = get(hf_sep, eol);
    write(fd, resp_str.c_str(), resp_str.size());
	return 0;
}