#include "client.hpp"

void        client::reset_time() // reset the time
{
    gettimeofday(&time, NULL);
    reset = false;
}

void    client::error(int status)
{
	if (fdbody != -1)
	{
		close(fdbody);
		fdbody = -1;
		FILE *file_body = tmpfile();
		fdbody = dup(fileno(file_body));
		fclose(file_body);
		file_body = NULL;
	}
	resp.clear();
	header.clear();
	func.clear();
	msg.clear();
	request req(header);
	response rep(status, req, *save_pars, fdbody, fdout);
	func = rep.get_func();
	header = rep.get_rep_header();
	resp += rep.get_rep_body();
}

bool	client::select_method(const fd_set &readfds, const fd_set &writefds)
{
	bool ret = false;
	if (func == "add_body")
		ret = add_body(readfds, writefds);
	else if (func == "put")
		ret = put(readfds, writefds);
	else if (func == "cgi")
		ret = method_cgi(readfds, writefds);
	else
		return false;
	if (ret)
		return true;
	func = "quit";
	return false;
}

void	client::del()
{
	if (my_cgi != NULL)
	{
		if (my_cgi->get_pid())
		{
			kill(my_cgi->get_pid(), SIGTERM);
			int status;
			waitpid(my_cgi->get_pid(), &status, 0);
		}
		delete(my_cgi);
		my_cgi = NULL;
	}
	if (fdbody != -1)
	{
		close(fdbody);
		fdbody = -1;
	}
	if (fdout != -1)
	{
		close(fdout);
		fdout = -1;
	}
}