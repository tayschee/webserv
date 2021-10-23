#include "webserv.hpp"

bool is_alive = 1;

void sighandler(const int signal) // catch the signals
{
	if (signal)
		is_alive = 0;
}

static int check_argument(int c)
{
	int i = 0;

	if (c != 2)
	{
		i = 1;
		std::cerr << "Invalid number of argument\n";
	}
	return i;
}

int main(int c, char **v)
{
	int ret = 0;
	int i = check_argument(c);

	if (i)
		return 1;
	signal(SIGINT, sighandler);
	signal(SIGPIPE, SIG_IGN);
	try
	{
		cluster cl(v[i + 1]);
		if (is_alive && cl.init_listen())
			if (is_alive && cl.start())
				return ret;
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		ret = 1;
	}
	catch(std::string &e) //remplace la string par une vrai exception ça serait un peut mieux
	{
		if (e == "quit cgi")
			ret = 500;
		std::cerr << e << std::endl;
	}
	return ret;
}
