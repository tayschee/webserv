#include <request.hpp>

/* read request of socket until the end */
void		request::read_socket(const int socket)
{
	const int buf_size = 4;
	char buffer[buf_size + 1];
	std::string	header_str;
	std::string	str_to_find(CRLF CRLF);
	size_t pos = 0;

	do
	{
		read(socket, buffer, buf_size);
		buffer[buf_size] = 0;
		header_str += buffer;
		pos = header_str.find(str_to_find); //WARNING read all each time must start to buf_size - 1
	}
	while (pos == header_str.npos); //read until double CRLF

	body = header_str.substr(pos + str_to_find.size());
	header_str = header_str.substr(0, pos);

	parse_header(header_str); //convert header_string to header
	read_end_of_body(socket); //read file until the end and put it into body
}

/*execute method store in method[CMD]*/
std::string	request::send_response() const
{
	response	resp(method, header, body);

	return (resp.message());
}