#include "webserv.hpp"
#include <iostream>

//std::string receive_func(int buffer_size, int fd)
//{
//	int i;
//	message::receive rcv(fd, buffer_size);
//	std::string msg;

//	while (1)
//	{
//		if (rcv() == -1)
//		{
//			throw msg;
//		}
//		if ((i= rcv.check()) & rcv.HEADER_MASK)
//		{
//			msg += rcv.get_buffer();
//		}
//		else if (i & rcv.BODY_MASK)
//		{
//			msg += rcv.get_buffer();
//			rcv.prepare_next();
//			break;
//		}
//	}
//	return msg;
//}

//int		main(int c, char **v)
//{
//	if (c != 3)
//	{
//		std::cout << "bad number of argument\n";
//		return 1;
//	}

//	int i = 0;
//	int number_of_test = atoi(v[2]);
//	int fd = open(v[1], O_RDONLY);
//	std::string msg_ref;
//	std::string msg;

//	msg_ref = receive_func(i, fd);
//	close(fd);
//	std::cout << "check if request is good and press enter\n";
//	std::cout << msg_ref << "\n";
//	read(1, NULL, 0);

//	while (i < number_of_test)
//	{
//		fd = open(v[1], O_RDONLY);
//		try
//		{
//			msg = receive_func(i, fd);
//		}
//		catch (std::string &msg)
//		{
//			std::cout << "eof\n";
//		}
//		close(fd);
//		if (msg != msg_ref)
//		{
//			std::cout << "-------------------\nGOOD REQUEST\n-----------------------\n" << msg_ref << "\n";
//			std::cout << "-------------------\nBAD REQUEST\n-----------------------\n" << msg << "\n";
//			std::cout << "--------------------\n" << "i : " << i << "\n";
//			return 1;
//		}
//		++i;
//	}
//	return 0;
//}

int main()
{
	std::vector<parser::address_conf> p = parser::parse_folder("conf/conf2");
	(void)p;

	return (0);
}
