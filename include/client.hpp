#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <time.h>
# include <sys/time.h>
# include "server.hpp"
# include "../include/message/request.hpp"
# include "../include/message/message.hpp"
# include "../include/message/response.hpp"
# include "../include/parser.hpp"

class client
{
    private: /*private function*/
        int                             fd;
        struct timeval                  time;
        bool                            listen; // is socket for listening or a new client
        int                             my_read;   // finished to read or not 
        int					            nb_pars;
        message::receive    			rcv;
        int                             fdin;
        FILE                            *file;
        size_t                          size_body;
        bool                            reset;


        client();

    public: /*public function*/
		bool			is_close;
        client(const int pfd, const bool _listen, int _nb_pars);
        client(int _fd, bool _listen, const client &other);

        client(const client& other);
        client& operator=(const client& other);
        ~client();

        void                            reset_time(); // Set the time at the current time
        int                             get_fd() const;
        int                             get_nb_pars() const;
        void                            receive(); // manage receive
        long                            sent(const std::vector<parser::address_conf> &vec_parser); // send response
        bool                            is_listen() const; // Check if listening socket
        bool                            is_time() const; // Check if the time is finished
        int                             is_read() const; // Check if read
        bool                            is_reset() const; // Check if read
		void							reset_rcv(size_t buf_size);
};

#endif