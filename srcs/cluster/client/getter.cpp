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

int         client::get_fdbody() const
{
    return fdbody;
}

int         client::get_fdin() const
{
    return fdin;
}

int         client::get_fdout() const
{
    return fdout;
}

void    client::parse_body(std::string &request_str, std::string &header)
{
	request_str.erase(0, skip(request_str, CRLF));
	std::string	sep(SEPARATOR);
    std::string body;

	size_t pos(request_str.find(sep));

	if (pos != request_str.npos)
	{
		
        header = request_str.substr(0, pos);
        request_str.erase(0, pos + sep.size());
	}
}

// int         client::get_pipe_in_0() const
// {
//     return pipe_in[0];
// }

// int         client::get_pipe_in_1() const
// {
//     return pipe_in[1];
// }

// int         client::get_pipe_out_0() const
// {
//     return pipe_out[0];
// }

// int         client::get_pipe_out_1() const
// {
//     return pipe_out[1];
// }

int         client::get_nb_pars() const
{
    return nb_pars;
}

int         client::get_first() const
{
    return first;
}