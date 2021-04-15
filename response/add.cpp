#include <request.hpp>

/*This file regroup all functions add_*, they are all private and are use to complete header header field by header field */

/*add field Allow inside header*/
void			request::response::add_allow(const std::string *allow_method_array)
{
	std::string allow_method_string;
	int i;

	/*concatenate allow methods and add " ," beetween them*/
	for (i = 0; allow_method_array[i] != ""; i++) //can be optimised
	{
		allow_method_string += allow_method_array[i];
		if (allow_method_array[i + 1] == "")
			break ;
		allow_method_string += ", ";
	}

	header.insert(std::pair<std::string, std::string>(ALLOW, allow_method_string));
}

/*add field Date to response::header*/
void			request::response::add_date()
{
	const std::string date = time_string();

	header.insert(std::pair<std::string, std::string>(DATE, date));
}

/*add field Content_Length to response::header*/
void			request::response::add_content_length(const header_type &req_head, const off_t &bytes_size)
{
	/*doc precise Content-Length MUST NOT be sent if there is a Transfert-Encoding field inside request, so we verify it*/
	if (req_head.find(TRANSFERT_ENCODING) == req_head.end())
	{
		/*Warning dont know of type off_t is larger than int*/
		header.insert(std::pair<std::string, std::string>(CONTENT_LENGTH, ft_itoa(bytes_size)));
	}
}

/*add field Last_Modified to response::header*/
void			request::response::add_last_modified(time_t time)
{
	header.insert(std::pair<std::string, std::string>(LAST_MODIFIED, time_string(time)));
}

/*add field Last_Modified to response::header, those function may change to replace "webserv" by server_name*/
void			request::response::add_server()
{
	header.insert(std::pair<std::string, std::string>(SERVER, WEBSERV));
}

void			request::response::add_content_type(const std::string &file)
{
	std::string extension;
	size_t		pos = file.find_first_of(".");
	value_type	media_type(DEFAULT_SUBTYPE, DEFAULT_TYPE);
	std::string value;

	if (pos != file.npos)
	{
 		extension = file.substr(pos + 1);
		media_type = get_media_type(extension);
		value = media_type.second + media_type.first + "; charset=UTF-8 ";   
	}
	header.insert(value_type(CONTENT_TYPE, media_type.second + media_type.first));
}

/*void				request::response::add_transfert_encoding(const std::string &file)
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

/* this time, this is not a field it's the body of response which be add */
void			request::response::add_body(int fd, struct stat file_stat)
{
	char	buffer[file_stat.st_size + 1]; //st_size contains size of file + 1 for '\0'

	/*read fd and put it's content in buffer*/
	if (read(fd, buffer, file_stat.st_size) < 0) 
	{
		return ; //add error
	}
	buffer[file_stat.st_size] = 0;

	body = buffer;
}