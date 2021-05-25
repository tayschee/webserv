#include "client.hpp"

bool        client::is_empty() const // check if requete is empty
{
	return req.get_method().empty(); // if method is void / close client
}

bool        client::is_listen() const // Check if listening socket
{
    return listen;
}

int         client::is_read() const // Check if the time is finished
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

int         client::get_fd() const
{
    return fd;
}