#ifndef SERVER_HPP
# define SERVER_HPP

/*Include below, put a comment next to it to say what function we use inside*/
# include <cstring> // strerror
# include <errno.h> // errno
# include <netinet/ip.h> // sockaddr_in
# include <sys/socket.h> // socket, bind, setsockopt, accept

# include "parser.hpp"

class   server
{
    public : /*typedef if neccessary*/

    private : /*private variables we can put a comment next to them to understand their utility*/

        int						port; // stock port
        uint32_t				ip; // stock ip
        const std::string		domain_name; //to replace ip adress
		int						socket_host; //to identify host
		sockaddr_in				address; // stock address

    private : /*private function*/
		server(); // cannot declare a server without port/ip...
		server(const server& other); // cannot copy server
		server& operator=(const server& other); // cannot copy server

    public : /*public function*/

		server(const parser & _parse); // constructor used
		~server(); // destructor

		int						get_socket_host() const; // get host
};

#endif
