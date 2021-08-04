#include "webserv.hpp"

static int check_argument(int c, char **v)
{
	int i;

	if (c < 2 || c > 3)
	{
		i = -1;
		std::cout << "Invalid number of argument\n";
	}
	else if (c == 2)
	{
		i = 0;
	}
	else
	{
		if (!strcmp(v[1], "-d") || !strcmp(v[1], "--debug"))
		{
			i = 1;
		}
		else
		{
			i = -1;
			std::cout << "Illegal option\nusage: webserv [-d] or [--debug] [directory]\n";
		}
	}
	return i;
}

int main(int c, char **v)
{
	int i = check_argument(c, v);
	bool debug;

	if (i < 0) //bad arguments
		return 1;

	debug = i; //define debug mode or not

	try
	{
		cluster cl(v[i], debug);
		cl.init_listen();
		cl.start();
	}
	catch(std::exception &e)
	{
		std::cerr << "MUST CHANGE THIS PART\n";
	}

	return 0;
}