#include "message/response.hpp"

/*Convert header_type to a syntax adapt for http do same thing than (std::string std::string::operator=(const header_type &)),
that doesn't exist for now, but it can be implement*/
std::string		response::header_to_string() const
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
void			response::main_header(const std::vector<std::string> &allow_method)
{
	add_allow(allow_method); // allow_field add in header
	add_date(); //date field add in header
	add_server(); //server field add in header
}

/*add inside response:header all field which are in all method and in all condition without allow use for error constructor*/
void			response::main_header()
{
	add_date(); //date field add in header
	add_server(); //server field add in header
}

/*create first line of response header */ 
std::string		response::header_first_line() const
{
	std::string		str_first_line;

	str_first_line = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.version + CRLF;

	return str_first_line;
}

/*parse value of accept* header-field*/
std::multimap<int, std::string>	response::tag_priority(std::string tag) const
{
	const char tag_sep[] = ",";
	const char value_sep[] = ";q=";
	tag = string_without(tag, " \t"); //delete one of those elem in string
	std::vector<std::string> split_tag(split(tag, tag_sep));
	std::vector<std::string>::const_iterator it(split_tag.begin());
	std::vector<std::string>::const_iterator end(split_tag.end());
	std::multimap<int, std::string>			 map;

	while (it < end)
	{
		size_t pos;
		const std::string key_tag(*it);

		if ((pos = key_tag.find(value_sep)) != key_tag.npos)
		{
			map.insert(std::map<int, std::string>::value_type
			(ft_atoi<float>(key_tag.substr(pos + strlen(value_sep))) * 100, key_tag.substr(0, pos)));
		}
		else
		{
			map.insert(std::map<int, std::string>::value_type(1 * 100, key_tag));
		}
		++it;
	}
	return map;
}

// Check authorizations
bool	response::is_authorize(const request &req, const parser &pars) const
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

void		response::status_header()
{
	if (first_line.status == 401)
		add_www_autentificate();
	if (first_line.status == 503)
		add_retry_after(200);
	if (first_line.status > 299 && first_line.status < 400)
		add_retry_after(1);
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
	if (path.empty())
		return 404;
	if (lstat(path.c_str(), &file_stat) < 0)
		return 404;
	if (int ret = is_open(file_stat))
		return ret;
	if (!is_authorize(req, pars))
		return 401;
	return (0);
}

// Check if the type is a CGI
bool		response::is_cgi(const std::string &type, const parser &pars) const
{
	if (type == ".bla")
		return false;
	try
	{
		pars.get_block("cgi", type);
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
void	response::get_code(const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	if (first_line.status == 401)
		header.insert(value_type(WWW_AUTHENTICATE, "Basic realm=\"Accès au site de webserv\", charset=\"UTF-8\""));
	if (first_line.status == 503)
		header.insert(value_type("Retry-after",  "20000"));
	std::string file_error = "/Users/jelarose/Documents/webserv-tbigot3/error/" + std::string(ft_itoa(first_line.status)) + ".html";
	if (lstat(file_error.c_str(), &file_stat) < 0)
	{
		file_error = "/Users/jelarose/Documents/webserv-tbigot3/error/404.html";
		lstat(file_error.c_str(), &file_stat);
	}
	int fd = open(file_error.c_str(), O_RDONLY);
	char buf[file_stat.st_size + 1];
	int ret = read(fd, buf, file_stat.st_size);
	buf[ret] = '\0';
	body = buf;
	header.insert(value_type(CONTENT_LENGTH,  ft_itoa(body.size())));
	header.insert(value_type(CONTENT_TYPE,  "text/html"));
	close(fd);
}

// Manage redirections
bool		response::is_redirect(parser::entries &block, const parser &pars)
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
}

std::string			response::ft_itoa_base(long nb, std::string &base)
{
	std::string ret;
	
	if (nb == 0)
		return "0";
	else if (nb < 0)
	{
		nb *= -1;
		ret = "-";
	}
	if (nb / base.size() > 0)
		ret += ft_itoa_base(nb / base.size(), base);
	ret += base[nb % base.size()];

	return ret;
}