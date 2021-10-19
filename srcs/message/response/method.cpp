#include "message/response.hpp"

int response::method_is_head(const std::string &uri, const request &req, const parser &pars)
{
	int ret = method_is_get(uri, req, pars);
	body.clear();
	header.erase(TRANSFERT_ENCODING);
	func.clear();
	return ret; //value of OK response
}

int response::method_is_get(const std::string &uri, const request &req, const parser &pars)
{
	struct stat file_stat; //information about file
	std::string path = find_path(pars, uri, req);
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
			{
				return 404;
			}
		}
		catch (const std::exception &e)
		{
			return 404;
		}
		body = index(path, uri);
		add_transfert_encoding();
		add_content_type(TEXT_HTML);
		func = "index";

		return 200;
	}
	else if (is_cgi(get_extension(path), pars, req.get_method()))
	{
		first_line.status = 42;
		method_function method = existing_method.find(POST)->second;
		return (this->*method)(path, req, pars); //change for if there is redirect
		add_last_modified(file_stat.st_mtime);	 /* st_mtime = hour of last modification */
		add_content_length(body.size());
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
		add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	}
	return 200;
}

int response::method_is_delete(const std::string &uri, const request &req, const parser &pars)
{
	std::string path = find_path(pars, uri, req, 0);
	struct stat file_stat;
	int ret = 0;

	std::string root = pars.get_block(BLOCK_LOCATION, uri).args[0];

	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	ret = check_path(path, file_stat, req, pars);
	if (ret == 403)
	{
		if (root == uri || root + "/" == uri)
			return 500;
		if (path.size() > 0 && *--path.end() != '/')
			return 404;
	}
	if (ret != 0)
		return ret;

	ret = del_content(path, req, pars, 0);
	if (ret != 0)
		return 404;
	ret = del_content(path, req, pars);
	if (ret != 0)
		return 404;
	return 204;
}

int response::method_is_post(const std::string &uri, const request &req, const parser &pars)
{
	std::string path;
	struct stat file_stat;

	if (first_line.status == 42)
		path = uri;
	else
		path = find_path(pars, uri, req);
	int ret = 0;
	if ((ret = is_authorize(uri, req, pars)))
		return ret;
	ret = check_path(path, file_stat, req, pars);
	if (ret == 403)
	{
		if (path.size() > 0 && *--path.end() != '/')
			return 404;
	}
	if (ret != 0)
		return ret;

	func = "cgi";
	if (!is_cgi(get_extension(path), pars, req.get_method()))
	{
		return 405;
	}
	save_path = path;

	add_content_type(TEXT_HTML + std::string("; ") + CHARSET_UTF8);
	if (req.get_method() != HEAD)
		add_transfert_encoding();

	return 200;
}

int response::method_is_unknow(const std::string &uri, const request &req, const parser &pars)
{
	(void)pars;
	(void)req;
	(void)uri;

	//Method Not Allowed
	return 405;
}