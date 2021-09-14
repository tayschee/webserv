#include "message/response.hpp"

int response::method_is_head(const std::string &uri, const request &req, const parser &pars)
{
	int ret = method_is_get(uri, req, pars);
	body.clear();
	return ret; //value of OK response
}

int response::method_is_get(const std::string &uri, const request &req, const parser &pars)
{
	struct stat file_stat; //information about file
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req);
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	ret = check_path(path, file_stat, req, pars);
	if (ret != 0)
		return ret;
	std::string type = find_media_type(get_extension(path), pars);
	if ((file_stat.st_mode & S_IFMT) == S_IFDIR /* || (file_stat.st_mode & S_IFMT) == S_IFLNK*/) //there is segfault on symbolic_link wihtout com
	{
		try
		{
			parser::block block(pars.get_block(BLOCK_LOCATION, req.get_uri()));
			parser::entries bc(block.conf);

			if (bc.find(AUTO_INDEX) != bc.end())
			{
				if (bc.find(AUTO_INDEX)->second[0] != "on")
				{
					std::string url;
					for (std::vector<std::string>::iterator it = block.args.begin(); it != block.args.end(); ++it)
						url += *it;
					if (req.get_uri().size() > 0 && *--req.get_uri().end() == '/')
						return 403;
					else
						return 404;
				}
			}
			else
				return 404;
		}
		catch (const std::exception &e)
		{
			return 404;
		}
		std::string add = (path.substr(pars.get_block(BLOCK_LOCATION, uri).conf.find(BLOCK_ROOT)->second[0].size()));
		body = index(path, uri, add);
		add_content_type(TEXT_HTML);
	}
	else if (is_cgi(get_extension(path), pars, req.get_method()))
	{
		first_line.status = 42;
		method_function method = existing_method.find(POST)->second;
		return (this->*method)(path, req, pars); //change for if there is redirect

		// status = (this->*method)(path, req, pars); //change for if there is redirect
	}
	else
	{
		if ((ret = add_body(path)) != 0)
			return ret;
		else if (type.empty())
		{
			add_content_type(APP_OCT_STREAM);
		}
		else
			add_content_type(type);
	}
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_length(body.size());

	return 200;
}

int response::method_is_delete(const std::string &uri, const request &req, const parser &pars)
{
	int ret;
	std::string path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req, 0);
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	ret = del_content(path, req, pars, 0);
	if (ret != 0)
		return ret;
	ret = del_content(path, req, pars);
	if (ret != 0)
		return ret;
	return 204;
}

int response::method_is_put(const std::string &uri, const request &req, const parser &pars)
{
	int fd;
	int response_value = 204;

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
			response_value = 201;										 //CREATE
			if ((fd = open(path.c_str(), O_WRONLY | O_CREAT, 0666)) < 0) //content doesn't exist so create it
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
		if (is_acces(file_stat))
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
	add_content_type(TEXT_HTML);

	return response_value;
}

int response::method_is_post(const std::string &uri, const request &req, const parser &pars)
{
	std::string path;

	if (first_line.status == 42)
		path = uri;
	else
		path = find_path(pars.get_block(BLOCK_LOCATION, uri), uri, req);
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;

	if (is_cgi(get_extension(path), pars, req.get_method()))
	{
		cgi(req, pars, body, path);
		if (body[0] == '5')
			return ft_atoi<int>(body);
	}
	add_content_type(TEXT_HTML + std::string("; ") + CHARSET_UTF8);
	add_content_length(body.size());

	return 200;
}

int response::method_is_unknow(const std::string &uri, const request &req, const parser &pars)
{
	(void)pars;
	(void)req;
	(void)uri;
	header.erase(GET);

	//Method Not Allowed
	return 405;
}