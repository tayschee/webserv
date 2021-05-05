#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <time.h>
#include "../include/message/request.hpp"
#include "../include/message/message.hpp"
#include "../include/message/response.hpp"
#include "../include/parser.hpp"

class client
{
private: /*private function*/
    int             fd;
    struct timeval  time;
    bool            listen;
    parser          pars;

public: /*public function*/
    client(const client& other); // cannot copy client
    client();
    client(int pfd, bool _listen, const parser &_pars);
    ~client();
	client& operator=(const client& other); // cannot copy client

    bool    is_time(); // Check if the time is finished
    void    set_time(); // Set the time at the current time
    int     &get_fd();
    parser  &get_pars();
    bool    is_listen();
    bool    is_read;
    message::receive_management rcm;
    request rq;
};

#endif