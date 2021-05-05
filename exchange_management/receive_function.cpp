#include "message/exchange_management.hpp"

typedef message::receive_management receive_management;
/*this file contain receive_management main functions*/

/*this function call by receive request is used to change type of read (header, body) to store all request correctly */
int		receive_management::receive(const int socket, message *req)
{
	int i;

	if ((i = data->receive(socket, req)) == 1)
	{
		if (dynamic_cast<receive_header *>(data) != NULL)
		{
			i = specialization(req);
			i = data->check(req);
		}
		else
			clear();
	}
	else if (i == -1)
		clear();
	return i;
}

int		receive_management::specialization(message *req)
{
	receive_management::internal_receive *new_data;
	iterator it(req->header.find(TRANSFERT_ENCODING));
	const_iterator end(req->header.end());

	if (it != end)
	{
		new_data = new(std::nothrow) receive_tf(this->data->msg); //if allocation fail return NULL
	}
	else if ((it = it = req->header.find(CONTENT_LENGTH)) != end)
	{
		new_data = new(std::nothrow) receive_cl(*this->data, ft_atoi<size_t>(it->second)); //if allocation fail return NULL
	}
	else
	{
		new_data = new(std::nothrow) receive_cl(*this->data, 0); //if allocation fail return NULL
	}
	delete data;
	data = new_data;
	return 1;
}

receive_management::internal_receive *receive_management::clone() const
{
	if (data == NULL)
		return data;
	return data->clone();
}

void	receive_management::clear()
{
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
}