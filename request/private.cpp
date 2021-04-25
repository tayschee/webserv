#include <message/request.hpp>

/* header is read, now read end of body */
/*void			request::read_end_of_body(const int socket)
{
	iterator end = header.end();
	iterator it(header.find(CONTENT_LENGTH));

	if (it != end) //is there a body ? 
	{
		//there is body
		size_t body_total_size = stoi(it->second);

		if (body.size() != body_total_size) //is there again something to read ?
		{
			//yes
			size_t buf_size = stoi(it->second) - body.size();
			char buffer[buf_size + 1];

			read(socket, buffer, buf_size);
			buffer[buf_size] = 0;
			body += buffer;
		}
	}
}*/

