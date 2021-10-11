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
		fclose(file_body);
		close(fdbody);
		fdbody = -1;
		file_body = tmpfile();
		fdbody = fileno(file_body);
	}
	resp.clear();
	func.clear();
	msg.clear();
	request req(header);
	response rep(status, req, *save_pars, fdbody, fdin, fdout);
	func = rep.get_func();
	resp = rep.get_rep_header();
	resp += rep.get_rep_body();
}
