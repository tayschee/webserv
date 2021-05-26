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
	(void)allow_method;
	//add_allow(allow_method); // allow_field add in header
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

//this function van be put in utils.hpp, it gives list of files inside directory
std::list<std::string>	response::files_in_dir(const std::string &path) const
{
	DIR *directory = opendir(path.c_str());
	struct dirent *entry;
	std::list<std::string> files;

	if (directory == NULL)
	{
		//do something
	}
	while ((entry = readdir(directory)))
	{
		if (entry->d_type == DT_REG || entry->d_type == DT_LNK) //if this is s file
			files.push_back(entry->d_name);
	}
	closedir(directory);
	return files;
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
	std::cout << "TYPE = " << type << std::endl;
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
	std::string file_error = "/home/user42/42/webserv/error/" + std::string(ft_itoa(first_line.status)) + ".html";
	if (lstat(file_error.c_str(), &file_stat) < 0)
	{
		file_error = "/home/user42/42/webserv/error/404.html";
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