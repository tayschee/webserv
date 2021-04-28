#include "message/request.hpp"

/*void			check_end_of_tf_body(receive_management *recv_data, const size_t old_size, const int size_read)
{
	size_t	i;
	size_t	pos;
	size_t	CRLF_size(ft_strlen(CRLF));
	
	if (recv_data->size == 0)
		i = old_size;
	else
	{
		i = old_size + size_read;
		if (i > old_size + recv_data->size)
		{
			recv_data->size =  i - (old_size + recv_data->size)
			return ; //continue to read
		}
	}
	while ((pos = recv_data->msg.find(CRLF, i)) != recv_data->msg.npos)
	{
		recv_data->size = atoi(recv_data->msg.substr(i, pos));
		recv_data->msg.erase(i, pos + CRLF_size);
		if (recv_data->size == 0)
		{
			recv_data->msg.erase(i); //retire le \r\n inutile de fin si il a deja ete lu sinon il n'est pas lu
			body = recv_data->msg;
			recv->data->step = recv_data->BODY_DONE;
			return 0;
		}
		else if (recv_data->msg.size() > i + recv_data->size + CRLF_size)
		{
			i += recv_data->size + CRLF_size;
		}
		else
		{
			recv_data->size = recv_data->size 
		}
	}
	recv_data->size = 0;

}*/

void			request::check_end_of_cl_body(receive_management *recv_data)
{
	std::cout << "size :" << recv_data->size << "\nmsg size : " << recv_data->msg.size() << "\n";
	if (recv_data->size == 0) //if msg is superior or equal than Content-Length
	{
		recv_data->step = recv_data->BODY_DONE;
		body = recv_data->msg;
	}
}

/*if there is Transfert-Encoding inside header read body with this func*/
/*void			receive_tf_body(const int socket, receive_management *recv_data, const size_t buf_size)
{
	char 	*buffer;
	size_t	old_size(recv_data->msg.size());
	int		i;


	buffer = new char[buf_size + 1];
	if (i = read(socket, buffer, buf_size) < 0)
		return ; //error
	buffer[i] = 0;
	recv_data->msg += buffer;
	check_end_tf_body(recv_data, old_size);
	delete[] buffer;
}*/

/*if there is Content-Length inside header read body with this func*/
void			request::receive_cl_body(const int socket, receive_management *recv_data)
{
	const size_t buf_size = recv_data->size;
	char *buffer = new char[buf_size + 1];
	int ret;

	std::cout << "buf_size :" << buf_size << "\n";
	ret = read(socket, buffer, buf_size);
	buffer[ret] = 0;

	std::cout << "ret :" << ret << "\n";
	recv_data->size -= ret;
	recv_data->msg += buffer;

	delete[] buffer;

	check_end_of_cl_body(recv_data);
}

void	request::receive_body(const int socket, receive_management *recv_data, const size_t buf_size)
{
	(void)buf_size;
	if (recv_data->is_there_cl == recv_data->YES)
	{
		receive_cl_body(socket, recv_data);
	}
	/*else if (recv_data->cl_or_tf == recv_data->TF)
	{
		recv_tf_body(socket, recv_data, buf_size)
	}
	else
	{
		//error
	}*/
}

void			request::prepare_receive_body(receive_management *recv_data, size_t pos)
{
	const_iterator	it(header.find(CONTENT_LENGTH));
	const_iterator 	end(header.end());

	recv_data->msg.erase(0, pos + ft_strlen(SEPARATOR)); //body start after SEPARATOR so delete uneccessary \r\n\r\n
	
	recv_data->step = recv_data->HEADER_DONE;
	recv_data->is_there_cl = recv_data->YES; //there is Content-Length
	recv_data->size = ft_atoi<size_t>(it->second) - recv_data->msg.size();
	check_end_of_cl_body(recv_data);

	/*it = header.find(TRANSFERT_ENCODING);
	if (it != end)
	{
		if (recv_data->is_there_cl == recv_data->yes)
			return ; //return 400
		recv_data->is_there_tf = recv_data->NO; //there is Transfert-Encoding
		recv_data->size = 0;
		check_end_of_tf_body();
	}*/
}

/*part of function read socket*/
void	request::receive_header(const int socket, receive_management *recv_data, const size_t buf_size)
{
	char	*buffer;
	size_t	pos;
	int 	i;

	buffer = new char[buf_size + 1];
	if ((i = read(socket, buffer, buf_size)) < 0)
		return ; //throw exception
	buffer[i] = 0;
	recv_data->msg += buffer;
	if (recv_data->size != 0)
		pos = recv_data->msg.find(SEPARATOR, recv_data->size - 1); 
	else
		pos = recv_data->msg.find(SEPARATOR, recv_data->size); //header end with CRLF CRLF (SEPARATOR)
	recv_data->size += i;

	if (pos != recv_data->msg.npos) //if there is double /r/n prepare receive body
	{
		std::cout << recv_data->msg << "\n";
		parse_header(recv_data->msg.substr(0, pos)); //fill header + request_line
		prepare_receive_body(recv_data, pos); //set all variable to recv_data to read body
	}

	delete[] buffer;
}

bool		request::receive(const int socket, receive_management *recv_data, const size_t buf_size)
{
	std::cout << "ok\n";
	if (recv_data->step == recv_data->NOTHING_DONE)
	{
		receive_header(socket, recv_data, buf_size);
	}
	else if (recv_data->step == recv_data->HEADER_DONE)
	{
		std::cout << "here\n";
		receive_body(socket, recv_data, buf_size);
	}
	return 0;
}