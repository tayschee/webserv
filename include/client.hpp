#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <time.h>
#include <sys/time.h>
#include "../include/message/request.hpp"
#include "../include/message/message.hpp"
#include "../include/message/response.hpp"
#include "../include/parser.hpp"

class client
{
private: /*private function*/
    int                             fd;
    struct timeval                  time;
    bool                            listen; // is socket for listening or a new client
    bool                            read;   // finished to read or not 
    parser                          pars;
    message::receive_management     rcm;
    request                         req;

    client();

public: /*public function*/
    client(const int pfd, const bool _listen, const parser &_pars);
    client(const client& other);
	client& operator=(const client& other);
    ~client();

    int                             get_fd(); // get_fd
    parser                          &get_pars(); // get_pars
    message::receive_management     &get_rcm(); // get_receive_management
    request                         &get_req(); // get_request
    bool                            is_listen() const; // Check if listening socket
    bool                            is_time() const; // Check if the time is finished
    bool                            is_read() const; // Check if is already read
    void                            set_time(); // Set the time at the current time
    void                            set_read(bool _read); // Set if read or not read
};

#endif