#include "message/exchange.hpp"

typedef message::receive receive;

/* receive parent class of other receive class,
it used to receive header and body through is internal class and it variable which change type according to needs
(read header, cl_body, tf_body), it's also child class of exchange. It is used with function receive of request */

//receive_mangement constructor destructor

receive::receive(size_t fd, size_t buf_size) : exchange(fd), data(new header(buf_size)), buf_size(buf_size) {}

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
	i = check();
	return i;
}

int		receive::check()
{
	while (data->check() == 1)
	{
		if (dynamic_cast<body *>(data) == NULL) //if true this is header
		{
			receive::body	*new_data = data->next_step();

			delete data;
			data = new_data;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

receive::header		*receive::clone() const
{
	return data->clone();
}

void				receive::clear()
{
	delete data;
}

std::string			receive::get()
{
	std::string str(data->get());
	body *body_data(dynamic_cast<body *>(data));

	if (body_data != NULL) //if true this is NOT header
	{
		header *new_data;
		new_data = body_data->previous_step(buf_size);

		delete data;
		data = new_data;
	}
	return str;
}