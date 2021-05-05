#include "../include/client.hpp"
#include <sys/time.h>
#include <fstream>      // std::ofstream
#include <unistd.h>

bool    client::is_time()
{
    if (is_listen())
        return 0;
    struct timeval now;
    gettimeofday(&now, NULL);
	if ((now.tv_sec - time.tv_sec) > 60)
        return 1;
    return 0;
}

void    client::set_time()
{
    gettimeofday(&time, NULL);
}

bool    client::is_listen()
{
    return listen;
}

int     &client::get_fd()
{
    return fd;
}

parser     &client::get_pars()
{
    return pars;
}

