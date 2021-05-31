#include "message/exchange_management.hpp"

typedef message::receive_management receive_management;
/*this file contain receive_management main functions*/

/*this function call by receive request is used to change type of read (header, body) to store all request correctly */
int		receive_management::receive(const int socket)
{
	int i;

	i = data->receive(socket);
	if (i == -1)
	{
		clear();
		return i;
	}
	i = next_step();
	std::cout << "return i = " << i << "\n";
	return i;
}

int receive_management::next_step()
{
	//std::cout << data->check() << "\n";
	while (data->check())
	{
		if (dynamic_cast<receive_header *>(data) != NULL)
		{
			internal_receive *new_data(data->next_step());
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

std::string							receive_management::get_msg() const
{
	return data->msg;
}

receive_management::internal_receive *receive_management::clone() const
{
	if (data == NULL)
		return data;
	return data->clone();
}

void		receive_management::reset(size_t read_size)
{
	if (data != NULL)
	{
		clear();
	}
	data = new receive_header(read_size);
}

void	receive_management::clear()
{
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
}