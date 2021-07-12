#include "message/exchange.hpp"

/*tf_body struct, use to read body with Content-Length header, this struct is manipulated
by receive once header is read */

typedef message::receive receive;

//constructor destructor
receive::tf_body::tf_body(const std::string &msg, const size_t pos) : body(msg, 10, pos), msg_begin(message::header_is_end(msg) + ft_strlen(SEPARATOR)) {}
receive::tf_body::tf_body(const tf_body &x) : body(x), msg_begin(x.msg_begin) {}
receive::tf_body::~tf_body() {}

//operator =
receive::tf_body &receive::tf_body::operator=(const tf_body &x)
{
	msg = x.msg;
	buf_size = x.buf_size;
	pos = x.pos;
	msg_begin = x.msg_begin;

	return *this;
}

//read body and when end is reached return 1 to delete object inside message::receive
int receive::tf_body::receive(const int socket)
{
	char *buffer;
	ssize_t i;

	buffer = new char[this->buf_size + 1];
	if ((i = read(socket, buffer, this->buf_size)) <= 0)
	{
		delete[] buffer;
		if (i == 0)
			return -1;
		else
			return 500; //throw
	}
	buffer[i] = 0;
	this->msg += buffer;
	delete[] buffer;
	this->buf_size -= i;
	return 0;
}

int receive::tf_body::check_end(const size_t i, const size_t CRLF_size)
{
	if (msg.size() >= i + (CRLF_size * 2))
	{
		if (msg_begin == pos)
		{
			msg.erase(pos, 1 + (CRLF_size * 2)); //erase 0\r\n + all after if there is
		}
		else
		{
			msg.erase(pos - CRLF_size, CRLF_size + 1 + (CRLF_size * 2)); //erase 0\r\n + all after if there is + \r\n before
			pos -= CRLF_size;
		}
		return 1;
	}
	else
	{
		if (i + CRLF_size - pos == 0)
			buf_size = 2;
		else
			buf_size = 1;
		return 0;
	}
}

size_t receive::tf_body::erase_tf_signature(const size_t i, const size_t CRLF_size, size_t pos)
{
	//std::cout << "before erase :" << msg.substr(pos) << "\n";
	if (msg_begin == pos) //find something
	{
		size_t nb_to_delete(i + CRLF_size - pos);

		msg.erase(pos, nb_to_delete); //delete length of current chunk
		return (pos);
	}
	else
	{
		//std::cout << "erase2\n" << "\n";
		pos = pos - CRLF_size;
		size_t nb_to_delete(i + CRLF_size - pos);

		msg.erase(pos, nb_to_delete); //delete length of current chunk + \r\n before, to have request without size
		return pos;
	}
}

//check if object have reached end of read body ("0" + CRLF) and return 1 if it's true
int receive::tf_body::check()
{
	size_t i;
	size_t CRLF_size(ft_strlen(CRLF));
	size_t msg_size(msg.size());
	const size_t default_buf_size = 7;

	if (!(pos >= msg.size()))
	{
		while ((i = msg.find(CRLF, pos)) != msg.npos) //check if there is CRLF
		{
			buf_size = ft_atoi_base<size_t>(msg.substr(pos), HEXADECIMAL_BASE); //store size of next buffer
			if (buf_size == 0)													//verify if it's end
			{
				return (check_end(pos, CRLF_size));
			}
			pos = erase_tf_signature(i, CRLF_size, pos);
			buf_size = buf_size + CRLF_size;
			pos += buf_size;
			msg_size = msg.size();
			if (msg_size < pos) //if size of buffer not again read
			{
				buf_size = pos - msg_size + default_buf_size;
				return 0;
			}
			buf_size = default_buf_size;
		}
	}
	if (buf_size == 0)
		buf_size = default_buf_size;
	return 0;
}

std::string receive::tf_body::get_buffer()
{
	size_t del_pos = msg.size() < pos ? msg.size() : pos;
	std::string buffer(msg.substr(0, del_pos));

	msg_begin = 5555; //never begin
	pos -= del_pos;
	msg.erase(0, del_pos);
	return buffer;
}

std::string receive::tf_body::get_header_buffer()
{
	size_t pos(header_is_end(msg));
	std::string header(msg.substr(0, pos));

	pos = pos + ft_strlen(SEPARATOR);
	msg.erase(0, pos);
	this->pos -= pos;
	msg_begin -= pos; //never begin

	return header;
}

receive::tf_body *receive::tf_body::clone() const
{
	tf_body *clone_obj;

	clone_obj = new tf_body(*this);
	return clone_obj;
}
