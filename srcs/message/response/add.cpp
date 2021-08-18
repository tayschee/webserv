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
	header.erase(CONTENT_LENGTH);
	header.insert(std::pair<std::string, std::string>(CONTENT_LENGTH, ft_itoa(bytes_size)));
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
	// (void)file;
	 //(void)req;
	header.erase(CONTENT_TYPE);
	header.insert(value_type(CONTENT_TYPE, type));


	// std::string extension;
	// size_t		pos = file.find_first_of(".");
	// request::header_type head = req.get_header();
	// request::header_type::const_iterator it(head.find(ACCEPT_CHARSET));

	// if (pos != file.npos)
	// {
 	// 	extension = file.substr(pos + 1);
	// 	media_type_array::value_type	media_type(find_media_type(extension));

	// 	if (it != head.end())
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first + ", " + it->second));
	// 	else
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first));
	// }
	// else
	// {
	// 	media_type_array::value_type	media_type(DEFAULT_SUBTYPE, DEFAULT_TYPE);

	// 	if (it != head.end())
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first + ", " + it->second));
	// 	else
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first));
	// }
}

/*void			response::add_content_type(const std::string &type) //pas tester
{
	// std::string extension;
	// size_t		pos = file.find_first_of(".");
	// request::header_type head = req.get_header();
	// request::header_type::const_iterator it(head.find(ACCEPT_CHARSET));

	// if (pos != file.npos)
	// {
 	// 	extension = file.substr(pos + 1);
	// 	media_type_array::value_type	media_type(find_media_type(extension));

	// 	if (it != head.end())
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first + ", " + it->second));
	// 	else
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first));
	// }
	// else
	// {
	// 	media_type_array::value_type	media_type(DEFAULT_SUBTYPE, DEFAULT_TYPE);

	// 	if (it != head.end())
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first + ", " + it->second));
	// 	else
	// 		header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first));
	// }
}*/

/*void				response::add_transfert_encoding(const std::string &file)
{
	std::string extension;
	size_t		pos = file.find_last_of(".");
	value_type	val;

	if (pos == file.npos)
		return ;	//no encoding

 	extension = file.substr(pos + 1);
	val = get_encoding_type(extension);

	header.insert(value_type(CONTENT_TYPE, val.first + val.second));
}*/

/* add field content_language, it s call by find_language if an appropriate language is find */
void				response::add_content_language(const std::string &language)
{
	header.insert(std::pair<std::string, std::string>(CONTENT_LANGUAGE, language));
}

void				response::add_www_autentificate()
{
	header.insert(value_type(WWW_AUTHENTICATE, "Basic realm=\"Accès au site de webserv\", charset=\"UTF-8\"")); //must change charset
}

/* this time, this is not a field it's the body of response which be add */

int			response::add_body(const std::string &path)
{
	char buf[4096] = {0};
	int fd;
	int res;
	if ((fd = open(path.c_str(), O_RDONLY)) < 0)
		return 403;
	while ((res = read(fd, buf, 4095)) > 0)
	{
		body.insert(body.end(), buf, buf + res);
		memset(buf, 0, 4096);
	}
	close(fd);

	return 0;
}
