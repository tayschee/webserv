#include "message/response.hpp"
#include <string>

int		response::method_is_head(const request &req, const parser &pars)
{
	int ret = method_is_get(req, pars);
	body.clear();
	add_content_length(0);
	return ret; //value of OK response
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

int		response::check_path(const std::string &path, struct stat &file_stat, const request &req, const parser &pars)
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

bool		is_cgi(const std::string &type, const parser &pars)
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

int		response::method_is_get(const request &req, const parser &pars)
{
	struct stat file_stat; //information about file
	std::string path = find_path(pars.get_block("location", req.get_uri()), req);
	//403 interdiction
	int ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;

	std::string type = find_media_type(get_extension(path), pars);
	
	if ((file_stat.st_mode & S_IFMT) == S_IFDIR || (file_stat.st_mode & S_IFMT) == S_IFLNK)
	{
		std::string add = (path.substr(pars.get_block("location", req.get_uri()).conf.find("root")->second[0].size()));
		body = index(path, req.get_uri(), add);
		add_content_type("text/html");
	}
	else if (is_cgi(get_extension(path), pars))
	{
		cgi(req, pars, body, path);
		if (body[0] == '5')
			return ft_atoi<int>(body);
		add_content_type("text/html");
	}
	else
	{
		if ((ret = add_body(path)) != 0)
			return ret;
		if (type.empty())
			add_content_type("application/octet-stream");
		else
			add_content_type(type);
	}
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_length(body.size());
	return 200;
}

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

int		response::method_is_delete(const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block("location", req.get_uri()), req, 0);
	int ret = del_content(path, req, pars, 0);
	if (ret != 0)
		return ret;
	ret = del_content(path, req, pars);
	if (ret != 0)
		return ret;
	return 204;
}

int		response::method_is_options(const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block("location", req.get_uri()), req, 0);
	struct stat file_stat; //information about file
	if (path.empty())
		return 404;
	if (stat(path.c_str(), &file_stat) < 0 && req.get_uri() != std::string("/*"))
		return 403;

	if (req.get_uri() == std::string("/*"))
	{
		parser::entries path_info(pars.get_block("server").conf);
		if (path_info.find("accept") != path_info.end())
		{
			std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
			add_allow(allow_method);
		}
	}
	else
	{
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, req.get_uri()).conf);
		std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
		add_allow(allow_method);
	}
	add_content_length(0);
	return 200; //value of OK response
}

int		response::method_is_put(const request &req, const parser &pars)
{
	int		fd;
	int		response_value = 204;

	std::string path = find_path(pars.get_block("location", req.get_uri()), req);
	struct stat file_stat; //information about file
	if (!is_authorize(req, pars))
		return 401;

	/*verify if content exist*/
	if (lstat(path.c_str(), &file_stat) < 0)
	{
		if ((fd = open(path.c_str(), O_WRONLY)) < 0)
		{
			response_value = 201; //CREATE
			if ((fd = open(path.c_str(), O_WRONLY | O_CREAT)) < 0) //content doesn't exist so create it
			{
				close(fd);
				return 403;
			}
			if (write(fd, req.get_body().c_str(), req.get_body().size()) < 0)
			{
				close(fd);
				return 403;
			}
			/* this header field are specific if file didn't exists */
			add_content_length(0);
			close(fd);
		}
	}
	else
	{
		if (int ret = is_open(file_stat))
			return ret;
		if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC)) < 0) //content doesn't exist so create it
		{
			close(fd);
			return 403;
		}
		if (write(fd, req.get_body().c_str(), req.get_body().size()) < 0)
		{
			close(fd);
			return 403;
		}
		/* this header field are specific if file didn't exists */
		response_value = 204; //CREATE
		close(fd);
	}
	header.insert(value_type(CONTENT_LOCATION, req.get_uri()));
	add_content_type("text/html");

	return response_value;
}

int		response::method_is_post(const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block("location", req.get_uri()), req);
	struct stat file_stat; //information about file
	int ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;
	cgi(req, pars, body, path);
	if (body[0] == '5')
		return ft_atoi<int>(body);
	add_content_type("text/html");
	add_content_length(body.size());

	return 200;
}

int		response::method_is_trace(const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block("location", req.get_uri()), req);
	struct stat file_stat; //information about file
	int ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;
	body = req.get_body();
	header.clear();
	message::header_type header_rcv = req.get_header();
	for (message::header_type::iterator it = header_rcv.begin(); it != header_rcv.end(); ++it)
		header.insert(value_type(it->first, it->second));
	header.erase(CONTENT_TYPE);
	header.erase(CONTENT_LENGTH);
	add_content_type("message/http");
	add_content_length(body.size());

	return 200;
}

int			response::method_is_unknow(const request &req, const parser &pars)
{
	std::cout << "JE SUIS DANS METHODE INCONNUS" << std::endl;
	(void)pars;
	(void)req;

	return 405; //Method Not Allowed
}