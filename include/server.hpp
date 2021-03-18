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

class   server
{
    public : /*typedef if neccessary*/

    private : /*private variables we can put a comment next to them to understand their utility*/

        int					port;
        uint32_t			ip;
        std::string			domain_name; //to replace ip adress
		int					socket_host; //to identify host
		std::vector<int>	socket_client; //to identify clients
		sockaddr_in			address;

    private : /*private function*/
		server(); // cannot declare a server without port/ip...
		server(const server& other); // cannot copy server
		server& operator=(const server& other); // cannot copy server

    public : /*public function*/
		server(const std::string& domain, const std::string& ip, int port);	
		~server();
		
		void start(); // Calls listen and starts the server
};

#endif
