#include "client.hpp"

#include <sys/types.h>
#include <sys/wait.h>

void client::receive(const fd_set &writefds) // call recieve
{
	long i;
	long ret = 0;
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
			my_read = 1;

			if (first)
			{
				parse_body(msg, header);
				first = false;
			}
			
			if (FD_ISSET(fdbody, &writefds))
			{
				if ((ret = write(fdbody, msg.c_str(), msg.size())))
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
			if (FD_ISSET(fdbody, &writefds))
			{
				if ((ret = write(fdbody, msg.c_str(), msg.size())))
					msg.clear();
			}
		}
	}
	reset = true;
}

long client::sent(const std::vector<parser::address_conf> &vec_parser, const fd_set &readfds, const fd_set &writefds) // send response
{
	if (!first && !msg.empty())
	{
		if (FD_ISSET(fdbody, &writefds))
		{
			long ret = 0;
			if ((ret = write(fdbody, msg.c_str(), msg.size())))
				msg.clear();
		}
	}
	else if (!first && msg.empty() && !header.empty())
	{
		first = true;
		lseek(fdbody, 0, SEEK_SET);
		request req(header);
		response rep(req, vec_parser[nb_pars], fdbody, fdin, fdout);
		save_path = rep.get_save_path();
		save_pars = rep.get_save_pars();
		func = rep.get_func();
		resp = rep.get_rep_header();
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
				fdout = fileno(file_out);
				my_cgi = new cgi(req, *save_pars, save_path, fdbody, fdout);
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
	else if (func == "add_body")
	{
		size_t size = 0;
		struct stat fd_stat;
		if (fstat(fdout, &fd_stat) == 0)
			size = fd_stat.st_size;
		if (!size)
		{
			func.clear();
		}
		else if (FD_ISSET(fdout, &readfds) && FD_ISSET(fd, &writefds))
		{
			int size_buffer = 0;
			__socklen_t	optlen;
			optlen = sizeof(size_buffer);
			getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
			if (size_buffer > 4097)
				size_buffer = 4097;
			else
				size_buffer = 2;
			char *buf = new char[size_buffer];
			int res;

			if ((res = read(fdout, buf, size_buffer)) > 0)
			{
				resp.insert(resp.end(), buf, buf + res);
				if (write(fd, resp.c_str(), resp.size()) < 0)
				{
					std::cout << "===================" << std::endl;
					std::cout << "PROBLEM WRITE ADD_BODY" << std::endl;
					std::cout << "===================" << std::endl;
					delete[](buf);
					buf = NULL;
					sleep(5);
					return 0;
				}
				resp.clear();
			}
			if (res < 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "PROBLEM ADD_BODY" << std::endl;
				std::cout << "===================" << std::endl;
				delete[](buf);
				buf = NULL;
				sleep(5);
				return 0;

			}
			if (res == 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "END ADD_BODY" << std::endl;
				std::cout << "===================" << std::endl;
				func.clear();
			}
			delete[](buf);
			buf = NULL;
		}
	}
	else if (func == "put")
	{
		size_t size_buffer = 0;
		size_t size = 0;
		struct stat fd_stat;
		if (fstat(fdbody, &fd_stat) == 0)
			size = fd_stat.st_size;
		if (!size)
		{
			func.clear();
		}
		else if (msg.empty() && FD_ISSET(fdbody, &readfds) && FD_ISSET(fdin, &writefds))
		{
			size_buffer = 4097;
			char *buf = new char[size_buffer];
			int res;
			size = 0;
			if (fstat(fdin, &fd_stat) == 0)
				size = fd_stat.st_size;

			while ((res = read(fdbody, buf, size_buffer - 1)) > 0)
			{
				buf[res] = 0;
				msg += buf;
				errno = 0;
				if (write(fdin, msg.c_str(), msg.size()) < 0)
				{
					std::cout << "===================" << std::endl;
					std::cout << strerror(errno) << std::endl;
					std::cout << "PROBLEM WRITE IN PUT" << std::endl;
					std::cout << "===================" << std::endl;
					delete[](buf);
					buf = NULL;
					sleep(5);
					return 0;
				}
				msg.clear();
			}
			if (res < 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "PROBLEM READ IN PUT" << std::endl;
				std::cout << "===================" << std::endl;
				delete[](buf);
				buf = NULL;
				sleep(5);
				return 0;

			}
			if (res == 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "END PUT" << std::endl;
				std::cout << "===================" << std::endl;
				func.clear();
			}
			delete[](buf);
			buf = NULL;
		}
		else if (!msg.empty() && FD_ISSET(fdin, &writefds))
		{
			if (write(fdin, msg.c_str(), msg.size()) < 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "PROBLEM WRITE IN PUT" << std::endl;
				std::cout << "===================" << std::endl;
				sleep(5);
				return 0;
			}
			msg.clear();
		}	
	}
	else if (func == "cgi")
	{
		if (my_cgi != NULL && FD_ISSET(fdout, &readfds) && FD_ISSET(fd, &writefds))
		{
			int status;
			if (!cgi_start)
			{
				if (waitpid(my_cgi->get_pid(), &status, WNOHANG))
				{
					if (WEXITSTATUS(status) == 244)
					{
						error(500);
						return 0;
					}
					lseek(fdout, 0, SEEK_SET);
					cgi_start = true;
				}
			}
			else
			{
				int size_buffer = 0;
				__socklen_t	optlen;
				optlen = sizeof(size_buffer);
				getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
				if (size_buffer > 131072)
					size_buffer = 131072;
				char *buf = new char[size_buffer];
				int ret;
				std::string tmp;
			
				if ((ret = read(fdout, buf, size_buffer - 1)) > 0)
				{
					buf[ret] = 0;
					tmp += buf;

					if (!h && (tmp.find("\r\n\r\n")) != std::string::npos)
					{
						tmp.erase(0, tmp.find("\r\n\r\n") + 4);
						h = 1;
					}
					if (h)
					{
						resp += ft_itoa_base(tmp.size(), HEXADECIMAL_BASE) + "\r\n" + tmp + "\r\n";
						tmp.clear();
						if ((ret = write(fd, resp.c_str(), resp.size())) < 0)
						{
							std::cout << "===================" << std::endl;
							std::cout << "PROBLEM WRITE CGI" << std::endl;
							std::cout << "===================" << std::endl;
							sleep(5);
							delete[](buf);
							buf = NULL;
							return 0;
						}
						resp.clear();
					}
				}
				if (ret < 0)
				{
					std::cout << "===================" << std::endl;
					std::cout << "PROBLEM READ CGI" << std::endl;
					std::cout << "===================" << std::endl;
					delete[](buf);
					buf = NULL;
					sleep(5);

					return 0;

				}
				if (ret == 0)
				{
					std::cout << "===================" << std::endl;
					std::cout << "END CGI" << std::endl;
					std::cout << "===================" << std::endl;
					resp += "0\r\n\r\n";
					func.clear();
					h = 0;
				}
				delete[](buf);
				buf = NULL;
			}
		}
	}
	else if (first && FD_ISSET(fd, &writefds) && func.empty())
	{
		size_t size_buffer = 0;
		__socklen_t	optlen;
		optlen = sizeof(size_buffer);
		getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
		if (size_buffer >= resp.size())
		{
			if (write(fd, resp.c_str(), resp.size()) < 0)
			{
				std::cout << "===================" << std::endl;
				std::cout << "PROBLEM WRITE IN SEND" << std::endl;
				std::cout << "===================" << std::endl;
				sleep(5);
				return 0;
			}
			msg.clear();
		}
		if (fdin != -1)
		{
			close(fdin);
			fdin = -1;
		}
		if (fdout != -1)
		{
			close(fdout);
			fdout = -1;
		}
		if (fdbody != -1)
		{
			fclose(file_body);
			close(fdbody);
			fdbody = -1;
			file_body = tmpfile();
			fdbody = fileno(file_body);
		}
		fclose(file_in);
		fclose(file_out);
		file_in = tmpfile();
		file_out = tmpfile();

		if (my_cgi != NULL)
		{
			delete(my_cgi);
			my_cgi = NULL;
		}
		if (!alive)
			return -1;
		
		cgi_start = false;
		first = true;
		resp = "";
		msg.clear();
		func.clear();
		my_read = 0;
		header.clear();
	}
	
	reset = true;
	return 0;
}