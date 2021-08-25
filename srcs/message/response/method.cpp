#include "message/response.hpp"

int		response::method_is_head(const std::string &uri, const request &req, const parser &pars)
{
	int ret = method_is_get(uri, req, pars);
	body.clear();
	//add_content_length(0);
	return ret; //value of OK response
}

int		response::method_is_get(const std::string &uri, const request &req, const parser &pars)
{
	std::cout << "it s ok\n";
	struct stat file_stat; //information about file
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req);
	std::cout << "path is : " << path << "\n";
	//403 interdiction
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;
	std::cout << "path is : " << path << "\n";
	std::string type = find_media_type(get_extension(path), pars);
	std::cout << "all fine\n";
	if ((file_stat.st_mode & S_IFMT) == S_IFDIR/* || (file_stat.st_mode & S_IFMT) == S_IFLNK*/) //there is segfault on symbolic_link wihtout com
	{
		try
		{
			parser::entries bc(pars.get_block(BLOCK_LOCATION, uri).conf);
			if (bc.find(AUTO_INDEX) != bc.end())
			{
				if (bc.find(AUTO_INDEX)->second[0] != "on")
					return 404;
			}
			else
				return 404;
		}
		catch(const std::exception& e)
		{
			std::cout << "execption" << std::endl;

			return 404;
		}
		std::cout << "all fine2\n";
		std::string add = (path.substr(pars.get_block(BLOCK_LOCATION, uri).conf.find(BLOCK_ROOT)->second[0].size()));
		std::cout << "all fine3\n";
		body = index(path, uri, add);
		add_content_type("text/html");
	}
	else
	{
		if ((ret = add_body(path)) != 0)
			return ret;		
		else if (type.empty())
			add_content_type("application/octet-stream");
		else
			add_content_type(type);
	}
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_length(body.size());

	return 200;
		// if (is_cgi(get_extension(path), pars, g))
		// 	add_content_type("text/plain");
	// else if (is_cgi(get_extension(path), pars))
	// {
	// 	cgi(req, pars, body, path);
	// 	if (body[0] == '5')
	// 		return ft_atoi<int>(body);
	// 	if (!req.get_tf().empty())
	// 		header.insert(value_type(std::string("Transfer-Encoding"), req.get_tf()));
	// 	add_content_type("text/html; charset=utf-8");
	// }
}

int		response::method_is_delete(const std::string &uri, const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req, 0);
	int ret = del_content(path, req, pars, 0);
	if (ret != 0)
		return ret;
	ret = del_content(path, req, pars);
	if (ret != 0)
		return ret;
	return 204;
}

int		response::method_is_options(const std::string &uri, const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req, 0);
	struct stat file_stat; //information about file
	if (path.empty())
		return 404;
	if (stat(path.c_str(), &file_stat) < 0 && uri != std::string("/*"))
		return 403;

	if (uri == std::string("/*"))
	{
		parser::entries path_info(pars.get_block(BLOCK_SERVER).conf);
		if (path_info.find(ACCEPT) != path_info.end())
		{
			std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
			add_allow(allow_method);
		}
	}
	else
	{
		parser::entries path_info(pars.get_block(BLOCK_LOCATION, uri).conf);
		std::vector<std::string> allow_method(path_info.find(ACCEPT)->second);
		add_allow(allow_method);
	}
	add_content_length(0);
	return 200; //value of OK response
}

int		response::method_is_put(const std::string &uri, const request &req, const parser &pars)
{
	int		fd;
	int		response_value = 204;

	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req);
	struct stat file_stat; //information about file
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	/*verify if content exist*/
	if (lstat(path.c_str(), &file_stat) < 0)
	{
		if ((fd = open(path.c_str(), O_WRONLY)) < 0)
		{
			response_value = 201; //CREATE
			if ((fd = open(path.c_str(), O_WRONLY | O_CREAT, 0666 )) < 0) //content doesn't exist so create it
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
		errno = 0;
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
	header.insert(value_type(CONTENT_LOCATION, uri));
	add_content_type("text/html");

	return response_value;
}

int		response::method_is_post(const std::string &uri, const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block("location", uri), uri, req);
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	// if (!is_cgi(get_extension(path), pars, req.get_method()))
	// 	return 404;
	// struct stat file_stat; //information about file
	// int ret = check_path(path, file_stat, req, pars);
	// if (ret != 0)
 	// 	return ret;
	// if ((file_stat.st_mode & S_IFMT) == S_IFDIR || (file_stat.st_mode & S_IFMT) == S_IFLNK)
	//   	return 404;
	
	if (is_cgi(get_extension(path), pars, req.get_method()))
	{
		cgi(req, pars, body, path);
		if (body[0] == '5')
			return ft_atoi<int>(body);
	}
	add_content_type("text/html; charset=utf-8");
	add_content_length(body.size());
	//add_content_type("application/octet-stream");
	//header.insert(value_type(std::string(TRANSFERT_ENCODING), std::string("chunked")));

	return 200;
}

int		response::method_is_trace(const std::string &uri, const request &req, const parser &pars)
{
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req);
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

int			response::method_is_unknow(const std::string &uri, const request &req, const parser &pars)
{
	std::cout << "JE SUIS DANS METHODE INCONNUS" << std::endl;
	(void)pars;
	(void)req;
	(void)uri;
	header.erase(GET);

	//Method Not Allowed
	return 405;
}