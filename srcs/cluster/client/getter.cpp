#include "client.hpp"

bool        client::is_listen() const // Check if listening socket
{
    return listen;
}

int         client::is_read() const // Check if the time is finished
{
    return my_read;
}

bool        client::is_reset() const // Check if reset
{
    return reset;
}

bool        client::is_time() const // Check if the time is finished
{
    if (is_listen() || my_read)
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

int         client::get_nb_pars() const
{
    return nb_pars;
}