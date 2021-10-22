#include "client.hpp"

bool client::add_body(const fd_set &readfds, const fd_set &writefds)
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
		socklen_t optlen;
		optlen = sizeof(size_buffer);
		getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
		if (size_buffer > 4096)
			size_buffer = 4096;
		char *buf = new char[size_buffer];
		int res;

		if ((res = read(fdout, buf, size_buffer)) > 0)
		{
			resp.insert(resp.end(), buf, buf + res);
			if (write(fd, resp.c_str(), resp.size()) <= 0)
			{
				delete[](buf);
				buf = NULL;
				return false;
			}
			resp.clear();
		}
		if (res < 0)
		{
			delete[](buf);
			buf = NULL;
			return false;
		}
		if (res == 0)
			func.clear();
		delete[](buf);
		buf = NULL;
	}
	return true;
}

bool client::method_cgi(const fd_set &readfds, const fd_set &writefds)
{

	if (my_cgi != NULL && FD_ISSET(fdout, &readfds) && FD_ISSET(fd, &writefds))
	{
		int status;
		if (!cgi_start)
		{
			if (my_cgi->get_pid() && waitpid(my_cgi->get_pid(), &status, WNOHANG))
			{
				my_cgi->reset_pid();
				if (WEXITSTATUS(status) == 244)
				{
					error(500);
					struct stat st;
					fstat(fdout, &st);
					return true;
				}
				lseek(fdout, 0, SEEK_SET);
				cgi_start = true;
			}
		}
		else
		{
			int size_buffer = 0;
			socklen_t optlen;
			optlen = sizeof(size_buffer);
			getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
			if (size_buffer > 4196)
				size_buffer = 4096;
			else
				return true;
			char *buf = new char[size_buffer + 1];
			int ret;
			// std::string tmp;

			if ((ret = read(fdout, buf, size_buffer)) > 0)
			{
				buf[ret] = 0;
				resp += buf;
				if (!h)
				{
					if (resp.find("\r\n\r\n") != std::string::npos)
					{
						h = 1;
						header += resp.substr(0, resp.find("\r\n\r\n") + 4);
						resp.erase(0, resp.find("\r\n\r\n") + 4);
						//  << "header:\n" << header;
						if (!header.empty())
							if (!send_header())
								return false;
						delete[](buf);
						buf = NULL;
						return true;
					}
					else
					{
						header += resp;
					}
				}
			}
			if (h && !resp.empty())
			{
				resp = ft_itoa_base(resp.size(), HEXADECIMAL_BASE) + "\r\n" + resp + "\r\n";
				if ((ret = write(fd, resp.c_str(), resp.size())) <= 0)
				{
					delete[](buf);
					buf = NULL;
					return false;
				}
				resp.clear();
			}
			if (ret < 0)
			{
				delete[](buf);
				buf = NULL;
				return false;
			}
			if (ret == 0)
			{
				resp += "0\r\n\r\n";
				func.clear();
				h = 0;
			}
			delete[](buf);
			buf = NULL;
		}
	}
	return true;
}

bool client::send_header()
{
	size_t size_buffer = 0;
	int ret = 0;
	socklen_t optlen;
	optlen = sizeof(size_buffer);
	getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);

	if (size_buffer > 4096)
		size_buffer = 4096;
	if (size_buffer > header.size())
		size_buffer = header.size();
	char *buf = new char[size_buffer];

	if ((ret = write(fd, header.c_str(), size_buffer)) <= 0)
	{
		delete[](buf);
		buf = NULL;
		return false;
	}
	header.erase(0, ret);
	delete[](buf);
	buf = NULL;
	return true;
}