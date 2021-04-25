#include <message/request.hpp>

/*part of function read socket*/
/*int		request::read_header(const int socket)
{
	const size_t buf_size = 4;
	char buffer[buf_size + 1];
	size_t pos;

	pos = body.size();
	if (read(socket, buffer, buf_size) < 0)
		return 1; //throw exception
	buffer[buf_size] = 0;
	body += buffer; //header is stock in body for now but that have no importance it's only during parsing
	if (pos != 0)
		pos = body.find(SEPARATOR, pos - 1); 
	else
		pos = body.find(SEPARATOR, pos); //header end with CRLF CRLF (SEPARATOR)
}*/
/* read request of socket until the end */
/*int		request::read_socket(const int socket)
{
	size_t pos;

	if (read_step == READING_HEADER)
	{
		pos = read_header(socket);
		if (pos != body.npos)
		{
			parse_header(body.substr(0, pos)); //fill header
			body = body.substr(pos + ft_strlen(SEPARATOR)); //body start after SEPARATOR
			read_step == READING_BODY;
		}
	}
	else if (read_step == READING_BODY)
	{
		if (body.size() < ft_itoa(header.find(CONTENT_LENGTH)->second))
		{

		}
	}
	read_end_of_body(socket); //read file until the end and put it into body
	return 0;
}*/

/*execute method store in method[CMD]*/
/*std::string	request::send_response() const
{
	response	resp(method, header, body);

	return (resp.message());
}*/