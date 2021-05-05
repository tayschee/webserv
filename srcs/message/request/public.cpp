#include "message/request.hpp"

bool		request::receive(const int socket, receive_management &recv_data)
{
	int i;
	std::cout << "111111111111111111111111" << std::endl;
	i = recv_data.receive(socket, this);
	std::cout << "222222222222222222222222" << std::endl;

	return i;
}