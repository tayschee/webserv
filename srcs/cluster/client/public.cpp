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
		msg = "";
	}
	else
	{
		i = rcv.check();
		if (i & rcv.BODY_MASK)
		{
			msg += rcv.get_buffer();
			size_body += msg.size();
			write(fdin, msg.c_str(), msg.size());
			msg.clear();
			msg = "";

			rcv.prepare_next();

			my_read = 1;
		}
		else if (i & rcv.HEADER_MASK)
		{
			msg += rcv.get_buffer();
			size_body += msg.size();

			write(fdin, msg.c_str(), msg.size());
			msg.clear();
			msg = "";
			my_read = 0;
		}
	}
	reset = true;
	// reset_time();
}

long client::sent(const std::vector<parser::address_conf> &vec_parser) // send response
{
	std::string msg;

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

	size_t i = 0;
	size_t j = 0;
	std::vector<parser> vec_pars = vec_parser[nb_pars];
	for (std::vector<parser>::iterator it2 = vec_pars.begin(); it2 != vec_pars.end(); ++it2)
	{
		if (it2->get_block("server").conf.find("server_name")->second[0] == "default")
			j = i;
		if (it2->get_block("server").conf.find("server_name")->second[0] == req.get_host().substr(0, req.get_host().find(":")))
			break;
		i++;
	}
	if (i == vec_pars.size())
		i = j;
	
	response rep(req, vec_parser.begin());
	my_read = rep.sent(fd);
	size_body = 0;
	reset = true;
	// reset_time();
	return 0;
}