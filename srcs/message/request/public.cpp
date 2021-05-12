#include "message/request.hpp"

int		request::receive(const int socket, receive_management &recv_data)
{
	int i;

	try
	{
		i = recv_data.receive(socket, this);
	}
	catch(const std::exception& e)
	{
		i = 500; //la valeur peut fluctuer
		recv_data.clear();
	}

	return i;
}