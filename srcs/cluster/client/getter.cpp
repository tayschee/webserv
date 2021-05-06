#include "client.hpp"

int         client::get_fd() // get_fd
{
    return fd;
}

parser     &client::get_pars() // get_pars
{
    return pars;
}

message::receive_management     &client::get_rcm() // get_receive_management
{
    return rcm;
}

request     &client::get_req() // get_request
{
    return req;
}

bool        client::is_listen() const // Check if listening socket
{
    return listen;
}

bool        client::is_read() const // Check if the time is finished
{
    return read;
}

bool        client::is_time() const // Check if the time is finished
{
    if (is_listen())
        return 0;
    struct timeval now;
    gettimeofday(&now, NULL);
	if ((now.tv_sec - time.tv_sec) > 60)
        return 1;
    return 0;
}

void        client::set_time() // Check if is already read
{
    gettimeofday(&time, NULL);
}

void        client::set_read(bool _read) // Set if read or not read
{
    read = _read;
}