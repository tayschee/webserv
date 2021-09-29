#include "client.hpp"

void client::receive() // call recieve
{
	int i;
	std::string msg;

	i = rcv();
	if (i == -1)
	{
		my_read = -1;
		msg.clear();
	}
	else
	{
		i = rcv.check();
		if (i & rcv.BODY_MASK)
		{
			msg += rcv.get_buffer();
			write(fdin, msg.c_str(), msg.size());
			msg.clear();

			rcv.prepare_next();

			my_read = 1;
		}
		else if (i & rcv.HEADER_MASK)
		{
			msg += rcv.get_buffer();

			write(fdin, msg.c_str(), msg.size());
			msg.clear();
			my_read = 0;
		}
	}
	reset = true;
}

int			client::add_body()
{
	char buf[100] = {0};
	int res;
	std::string body;
	
	if ((res = read(fd_response, buf, 99)) > 0)
	{
		body.insert(body.end(), buf, buf + res);
		memset(buf, 0, 100);
	}
	if (res == 0)
	{
		func = "";
		close(fd_response);
	}
	write (fd, body.c_str(), body.size());
	body.clear();
	return 0;
}

long client::sent(const std::vector<parser::address_conf> &vec_parser) // send response
{
	std::string msg;
	bool first = false;

	if (func.empty())
	{
		first = true;
		lseek(fdin, 0, SEEK_SET);
		size_t size = 4096;
		char buf[size + 1];
		int ret = 0;
		while ((ret = read(fdin, buf, size)) > 0)
		{
			buf[ret] = 0;
			msg += buf;
		}
		close(fdin);
		fclose(file);
		file = tmpfile();
		fdin = fileno(file);
		request req(msg);
		msg.clear();
		msg = "";
		response rep(req, vec_parser[nb_pars], fd);
		if (req.get_connexion() == "close")
			alive = false;
		func = rep.get_func();
		fd_response = rep.get_fd_response();
		if (func != "cgi run")
			rep.sent(fd, first);
		rep = response();
	}
	else
	{
		first = false;
		add_body();
	}
	if (func.empty() || func == "cgi run")
	{
		func.clear();
		fd_response = -1;
		my_read = 0;
		if (!alive)
			return -1;
	}
	if (func == "cgi end")
	{
		throw std::string("quit PHP");
	}
	reset = true;

	return 0;
}