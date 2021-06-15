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
        int                             read;   // finished to read or not 
        const parser::address_conf		*pars;
        message::receive    			rcv;
        request                         req;
        response                        rep;
		//int							fd_dest;

        void                            reset_time(); // Set the time at the current time

        client();

    public: /*public function*/
		bool			is_close;
        client(const int pfd, const bool _listen, const  parser::address_conf *_pars);
        client(int _fd, bool _listen, const client &other);

        client(const client& other);
        client& operator=(const client& other);
        ~client();

        int                             get_fd() const;
        void                            receive(); // manage receive
        long                            sent(); // send response
        bool                            is_empty() const;
        bool                            is_listen() const; // Check if listening socket
        bool                            is_time() const; // Check if the time is finished
        int                             is_read() const; // Check if read
		void							reset_rcv(size_t buf_size);
};

#endif