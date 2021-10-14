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
		__socklen_t optlen;
		optlen = sizeof(size_buffer);
		getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
		if (size_buffer > 4096)
			size_buffer = 4096;
		char *buf = new char[size_buffer];
		if (!buf)
			return false;
		int res;

		if ((res = read(fdout, buf, size_buffer)) > 0)
		{
			resp.insert(resp.end(), buf, buf + res);
			if (write(fd, resp.c_str(), resp.size()) <= 0)
			{
				if (buf)
				{
					delete[](buf);
					buf = NULL;
				}
				return false;
			}
			resp.clear();
		}
		if (res < 0)
		{
			if (buf)
			{
				delete[](buf);
				buf = NULL;
			}
			return false;
		}
		if (res == 0)
			func.clear();
		if (buf)
		{
			delete[](buf);
			buf = NULL;
		}
	}
	return true;
}

bool client::put(const fd_set &readfds, const fd_set &writefds)
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
	else if (msg.empty() && FD_ISSET(fdbody, &readfds) && FD_ISSET(fdout, &writefds))
	{
		size_buffer = 4096;
		char *buf = new char[size_buffer];
		if (!buf)
			return false;
		int res;
		size = 0;
		if (fstat(fdout, &fd_stat) == 0)
			size = fd_stat.st_size;

		while ((res = read(fdbody, buf, size_buffer - 1)) > 0)
		{
			buf[res] = 0;
			msg += buf;
			errno = 0;
			if (write(fdout, msg.c_str(), msg.size()) <= 0)
			{
				if (buf)
				{
					delete[](buf);
					buf = NULL;
				}
				return false;
			}
			msg.clear();
		}
		if (res < 0)
		{
			if (buf)
			{
				delete[](buf);
				buf = NULL;
			}
			return false;
		}
		if (res == 0)
			func.clear();
		if (buf)
		{
			delete[](buf);
			buf = NULL;
		}
	}
	else if (!msg.empty() && FD_ISSET(fdout, &writefds))
	{
		if (write(fdout, msg.c_str(), msg.size()) <= 0)
		{
			msg.clear();
			return false;
		}
		msg.clear();
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
				if (!header.empty())
					if (!send_header())
						return false;
				lseek(fdout, 0, SEEK_SET);
				cgi_start = true;
			}
		}
		else
		{
			int size_buffer = 0;
			__socklen_t optlen;
			optlen = sizeof(size_buffer);
			getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);
			if (size_buffer > 4096)
				size_buffer = 4096;
			char *buf = new char[size_buffer + 1];
			if (!buf)
				return false;
			int ret;
			std::string tmp;

			if ((ret = read(fdout, buf, size_buffer)) > 0)
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
					if ((ret = write(fd, resp.c_str(), resp.size())) <= 0)
					{
						if (buf)
						{
							delete[](buf);
							buf = NULL;
						}
						return false;
					}
					resp.clear();
				}
			}
			if (ret < 0)
			{
				if (buf)
				{
					delete[](buf);
					buf = NULL;
				}
				return false;
			}
			if (ret == 0)
			{
				resp += "0\r\n\r\n";
				func.clear();
				h = 0;
			}
			if (buf)
			{
				delete[](buf);
				buf = NULL;
			}
		}
	}
	return true;
}

bool client::send_header()
{
	size_t size_buffer = 0;
	int ret = 0;
	__socklen_t optlen;
	optlen = sizeof(size_buffer);
	getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size_buffer, &optlen);

	if (size_buffer > 4096)
		size_buffer = 4096;
	if (size_buffer > header.size())
		size_buffer = header.size();
	char *buf = new char[size_buffer];
	if (!buf)
		return false;

	if ((ret = write(fd, header.c_str(), size_buffer)) <= 0)
	{
		if (buf)
		{
			delete[](buf);
			buf = NULL;
		}
		return false;
	}
	header.erase(0, ret);
	if (buf)
	{
		delete[](buf);
		buf = NULL;
	}
	return true;
}