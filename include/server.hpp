#ifndef SERVER_HPP
# define SERVER_HPP

/*Include below, put a comment next to it to say what function we use inside*/
# include <string> //std::string

class   server
{
    public : /*typedef if neccessary*/

    private : /*private variables we can put a comment next to them to understand their utility*/

        /*int			port;
        int			ip;
        std::string	domain_name; //to replace ip adress
		int			socket_host; //to identify host
		int			socket_client; //to identify client*/


    private : /*private function*/

		template <typename Int_type>
		Int_type reverse_bytes_order(Int_type x) const;

		unsigned short ft_htons(unsigned short x) const;
		unsigned int ft_htonl(unsigned int x) const;

    public : /*public function*/

		server();
		~server();
		

};

#endif