#include "message/exchange.hpp"

#include <iostream>
#include <stdio.h>

typedef message::receive receive;

/* receive parent class of other receive class,
it used to receive header and body through is internal class and it variable which change type according to needs
(read header, cl_body, tf_body), it's also child class of exchange. It is used with function receive of request */

//receive_mangement constructor destructor

receive::receive(size_t fd, size_t buf_size) : exchange(fd), data(new header(buf_size)), buf_size(buf_size)
{}

receive::receive(const receive &x) : 
exchange(x), data(x.clone()), buf_size(x.buf_size){}

receive::~receive()
{
	clear();
}

//receive_mangement operator =
receive &receive::operator=(const receive &x)
{
	fd = x.fd;
	if (data != NULL)
		delete data;
	data = x.clone();
	buf_size = x.buf_size;
	return *this;
}


//MAIN FUNCTION

int receive::operator()()
{
	int i;
	i = data->receive(fd);
	if (i != 0)
		return i;
	//i = check(); //do it manually
	return i;
}

int		receive::check()
{
	int ret(dynamic_cast<body *>(data) == NULL ? NOTHING_END : HEADER_END);

	while (data->check() == 1)
	{
		if (ret == NOTHING_END) //if true this is header
		{
			receive::body	*new_data = data->next_step();
			delete data;
			data = new_data;
			ret = HEADER_END;
		}
		else
		{
			return BODY_END;
		}
	}
	return ret;
}

receive::header		*receive::clone() const
{
	return data->clone();
}

void				receive::clear()
{
	if (data != NULL)
		delete data;
	data = NULL;
}

std::string			receive::get_buffer()
{
	return data->get_buffer();
}

int			receive::get_size()
{
	return data->get_size();
}

std::string			receive::get_header_buffer()
{
	body *body_data(dynamic_cast<body *>(data));
	std::string str(body_data->get_header_buffer());

	return str;
}

void				receive::set_buf_size(size_t buf_size)
{
	this->buf_size = buf_size;
	header *head(dynamic_cast<body *>(data));

	if (head == NULL)
	{
		head->set_buf_size(buf_size);
	}
}

void				receive::prepare_next()
{
	body *body_data(dynamic_cast<body *>(data));

	if (body_data != NULL) //if true this is NOT header
	{
		header *new_data;
		new_data = body_data->previous_step(buf_size);

		delete data;
		data = new_data;
	}
}

void				receive::reset(const int fd, const size_t buf_size)
{
	delete data;

	this->fd = fd;
	this->buf_size = buf_size;
	data = new header(buf_size);
}