#include "message/response.hpp"

/*This file regroup all functions add_*, they are all private and are use to complete header header field by header field */

/*add field Allow inside header*/
void			response::add_allow(const std::vector<std::string> &allow_method_array)
{
	std::vector<std::string>::const_iterator it(allow_method_array.begin());
	std::vector<std::string>::const_iterator end(allow_method_array.end());
	std::string allow_method_string;

	/* concatenate allow methods and add " ," beetween them */
	while(it < end) //can be optimised
	{
		allow_method_string += *it;
		if (it + 1 == end)
			break ;
		allow_method_string += ", ";
		++it;
	}
	header.insert(std::pair<std::string, std::string>(ALLOW, allow_method_string));
}

/*add field Date to response::header*/
void			response::add_date()
{
	const std::string date = time_string();

	header.insert(std::pair<std::string, std::string>(DATE, date));
}

/*add field Content_Length to response::header*/
void			response::add_content_length(const off_t &bytes_size) //off_t == long long on mac
{
	/*Warning dont know of type off_t is larger than int*/
	// header.erase(CONTENT_LENGTH);
	// if (bytes_size > 0)
	header.insert(std::pair<std::string, std::string>(CONTENT_LENGTH, ft_itoa(bytes_size)));
}

void				response::add_www_autentificate(const parser &pars, const std::string &path)
{
	try
	{
		parser::block bloc = pars.get_block(BLOCK_LOCATION, path);
		if (bloc.conf.find(AUTH_BASIC) != bloc.conf.end())
		{
			std::string txt("Basic realm=");
			txt += pars.get_block(BLOCK_LOCATION, path).conf.find(AUTH_BASIC)->second[0];
			header.insert(value_type(WWW_AUTHENTICATE, txt)); //must change charset
		}
	}
	catch(const std::exception& e)
	{
	}	
}

/*add field Last_Modified to response::header*/
void			response::add_last_modified(time_t time)
{
	header.insert(std::pair<std::string, std::string>(LAST_MODIFIED, time_string(time)));
}

/*add field Last_Modified to response::header, those function may change to replace "webserv" by server_name*/
void			response::add_server()
{
	header.insert(std::pair<std::string, std::string>(SERVER, WEBSERV));
}

void			response::add_retry_after(size_t sec)
{
	header.insert(std::pair<std::string, std::string>(RETRY_AFTER, ft_itoa(sec)));
}

void			response::add_content_type(const std::string &type) //pas tester
{
	header.erase(CONTENT_TYPE);
	header.insert(value_type(CONTENT_TYPE, type));
}

/* add field content_language, it s call by find_language if an appropriate language is find */
void				response::add_content_language(const std::string &language)
{
	header.insert(std::pair<std::string, std::string>(CONTENT_LANGUAGE, language));
}

void				response::add_connection(int status, const request &req)
{
	if (status >= 500 || status == 400)
	{
		request &tmp = (request&)req;
		header.insert(std::pair<std::string, std::string>(CONNECTION, CLOSE));
		tmp.set_connexion(CLOSE);
	}
	message::const_iterator it = req.get_header().find(CONNECTION);

	if (it != req.get_header().end())
		header.insert(std::pair<std::string, std::string>(CONNECTION, it->second));
	else
	{
		request &tmp = (request&)req;
		header.insert(std::pair<std::string, std::string>(CONNECTION, KEEP_ALIVE));
		tmp.set_connexion(KEEP_ALIVE);
	}
}

void				response::add_transfert_encoding() //Transfert-Encoding
{
	header.insert(std::pair<std::string, std::string>(TRANSFERT_ENCODING, "chunked"));
}

/* this time, this is not a field it's the body of response which be add */

int response::add_body(const std::string &path)
{
	struct stat file_stat; //information about file
	// char buf[100] = {0};
	// int res;
	// if (func.empty())
	// {
	if ((fdout = open(path.c_str(), O_RDONLY)) < 0)
		return 403;
	// }
	// if ((res = read(fd_response, buf, 99)) > 0)
	// {
	// 	body.insert(body.end(), buf, buf + res);
	// 	memset(buf, 0, 100);
	// }
	stat(path.c_str(), &file_stat);
	add_content_length(file_stat.st_size);
	// if (res == 0 && func.empty())
	// {
	// 	func = "";
	// 	save_path = "";
	// 	close(fd_response);
	// 	return 0;
	// }
	save_path = path;
	func = "add_body";
	return 0;
}