#include "client.hpp"
#include <sys/types.h>
#include <sys/wait.h>

bool client::receive(const fd_set &writefds) // call recieve
{
	long i;
	long ret = 0;

	try
	{
		i = rcv();
		if (i == -1)
		{
			msg.clear();
			return false;
		}
		else
		{
			i = rcv.check();

			if (i & rcv.BODY_MASK)
			{
				msg += rcv.get_buffer();
				my_read = 1;

				if (first)
				{
					parse_body(msg, header);
					first = false;
				}
				if (FD_ISSET(fdbody, &writefds) && msg.size() > 0)
				{
					if ((ret = write(fdbody, msg.c_str(), msg.size())) <= 0)
						return false;
					msg.clear();
				}
				rcv.prepare_next();
			}
			else if (i & rcv.HEADER_MASK)
			{
				msg += rcv.get_buffer();

				if (first)
				{
					parse_body(msg, header);
					first = false;
				}
				if (FD_ISSET(fdbody, &writefds) && msg.size() > 0)
				{
					if ((ret = write(fdbody, msg.c_str(), msg.size())) <= 0)
						return false;
					msg.clear();
				}
			}
		}
		reset = true;
		return true;
	}
	catch(std::exception &e)
	{
		(void)e;
		return false;
	}
}

bool client::sent(const std::vector<parser::address_conf> &vec_parser, const fd_set &readfds, const fd_set &writefds) // send response
{
	try
	{
		if (!first && !msg.empty() && (FD_ISSET(fdbody, &writefds)))
		{
			size_t size_buffer = 0;
			int ret = 0;
			socklen_t optlen;
			optlen = sizeof(size_buffer);
			getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
			if (size_buffer > 4096)
				size_buffer = 4096;
			if (size_buffer > msg.size())
				size_buffer = msg.size();
			
			char *buf = new char[size_buffer];
			if (size_buffer > 0)
			{
				if ((ret = write(fd, msg.c_str(), size_buffer)) <= 0)
				{
					delete[](buf);
					buf = NULL;
					return false;
				}
			}
			msg.erase(0, ret);
			delete[](buf);
			buf = NULL;

			return true;
		}
		else if (!first && msg.empty() && !header.empty())
		{
			first = true;
			lseek(fdbody, 0, SEEK_SET);
			request req(header);
			header.clear();
			response rep(req, vec_parser[nb_pars], fdbody, fdout);
			if (req.get_connexion() == "close")
				alive = false;
			save_path = rep.get_save_path();
			save_pars = rep.get_save_pars();
			func = rep.get_func();
			header = rep.get_rep_header();
			msg = rep.get_rep_body();
			if (!msg.empty())
			{
				if (func == "index")
				{
					if (!msg.empty())
						resp += ft_itoa_base(msg.size(), HEXADECIMAL_BASE) + "\r\n" + msg;
					resp += "\r\n0\r\n\r\n";
				}
				else
					resp += msg;
				func.clear();
				msg.clear();
			}
			else if (func == "cgi" && !save_path.empty())
			{
				if (my_cgi == NULL)
				{
					FILE *file_out = tmpfile();
					fdout = dup(fileno(file_out));
					fclose(file_out);
					file_out = NULL;
					fcntl(fdout, F_SETFL, O_NONBLOCK);
					my_cgi = new cgi(req, *save_pars, save_path, fdbody, fdout, fd);
					if (!my_cgi)
						return false;
				}
			}
			else if (func == "cgi")
			{
				resp += "0\r\n\r\n";
				func.clear();
			}
			else if (func == "add_body")
				lseek(fdout, 0, SEEK_SET);
		}
		else if (first && !header.empty() && func != "cgi" && FD_ISSET(fd, &writefds))
		{
			return send_header();
		}
		else if (!select_method(readfds, writefds) && first && FD_ISSET(fd, &writefds) && func.empty())
		{
			if (func == "quit")
				return false;
			size_t size_buffer = 0;
			int ret = 0;
			socklen_t optlen;
			optlen = sizeof(size_buffer);
			getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);

			if (size_buffer > 4096)
				size_buffer = 4096;
			if (size_buffer > resp.size())
				size_buffer = resp.size();
			char *buf = new char[size_buffer];
			if (size_buffer > 0)
			{
				if ((ret = write(fd, resp.c_str(), resp.size())) <= 0)
				{
					delete[](buf);
					buf = NULL;
					return false;
				}
				resp.erase(0, ret);
				delete[](buf);
				buf = NULL;
			}
			if (!resp.empty())
				return true;
			delete[](buf);
			buf = NULL;
			del();
			FILE *file_body = tmpfile();
			fdbody = dup(fileno(file_body));
			fclose(file_body);
			file_body = NULL;

			if (!alive)
				return false;

			cgi_start = false;
			first = true;
			resp = "";
			msg.clear();
			func.clear();
			my_read = 0;
			header.clear();
		}
	}
	catch(std::exception &e)
	{
		(void)e;

		return false;
	}

	reset = true;
	return true;
}