#ifndef SERVER_HPP
# define SERVER_HPP

/*Include below, put a comment next to it to say what function we use inside*/
#include <string> //std::string

class   server
{
    public : /*typedef if neccessary*/

    private : /*private variables we can put a comment next to them to understand their utility*/

        int			port;
        uint32_t	ip;
        std::string	domain_name; //to replace ip adress
		int			socket_host; //to identify host
		int			socket_client; //to identify client


    private : /*private function*/
		server(); // cannot declare a server without port/ip...
		server(const server& other); // cannot copy server
		server& operator=(const server& other); // cannot copy server

    public : /*public function*/
		server(const std::string& domain, const std::string& ip, int port);	

};

#endif
