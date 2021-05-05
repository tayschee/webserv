#ifndef SERVER_HPP
# define SERVER_HPP

/*Include below, put a comment next to it to say what function we use inside*/
#include <sys/types.h>
#include <string> //std::string
#include <cstring> // strerror
#include <errno.h> // errno
#include <netinet/ip.h> // sockaddr_in
#include <sys/socket.h> // socket, bind, setsockopt, accept
#include <vector> // std::vector
#include <sys/select.h> // select, fd_set,
#include <sys/time.h> // FD_ZERO, FD_SET, FD_ISSET
#include <algorithm> // std::max, std::find
#include <unistd.h> // close
#include <iostream> //std::cerr

#include <list>
#include "client.hpp"
#include <csignal>
#include "parser.hpp"

class   server
{
    public : /*typedef if neccessary*/

    private : /*private variables we can put a comment next to them to understand their utility*/

        int					port;
        uint32_t			ip;
        std::string			domain_name; //to replace ip adress
		int					socket_host; //to identify host
		std::list<client>	socket_client; //to identify clients
		sockaddr_in			address;
		parser				pars;

    private : /*private function*/
		server(); // cannot declare a server without port/ip...
		server& operator=(const server& other); // cannot copy server

		template <typename Int_type>
		Int_type reverse_bytes_order(Int_type x) const; //convert whole number from little endian to big endian and vice versa

		unsigned short ft_htons(unsigned short x) const; //convert short for endian of network same behavior than real functions
		unsigned int ft_htonl(unsigned int x) const; //convert int for endian of network same behavior than real functions


    public : /*public function*/
		//int sighandler(const int signal, void *ptr);

		server(const server& other); // cannot copy server
		//std::list<client>::iterator		close_client(client cl);
		server(const parser & _parse);
		~server();

		int		get_socket_host()
		{
			return socket_host;
		}

		parser	get_parser()
		{
			return pars;
		}

		std::list<client>	get_list_client()
		{
			return socket_client;
		}

		//void start(); // Calls listen and starts the server
};

#endif
