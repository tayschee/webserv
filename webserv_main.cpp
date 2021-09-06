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

static int start(cluster &cl)
{
	int ret = 0;
	try
	{
		cl.start();
	}
	catch(std::string &e) //remplacela string par une vrai exception ça serait un peut mieux
	{
		std::cerr << e;
		ret = 1;
	}
	catch(std::exception &e)
	{
		std::cout << e.what();
		std::cerr << "something failed\n";
		ret = 1;
	}
	return ret;
}

int main(int c, char **v)
{
	int ret = 0;
	int i = check_argument(c, v);

	if (i < 0) //bad arguments
		return 1;

	try
	{
		cluster cl(v[i + 1], i);
		cl.init_listen();
		ret = start(cl);
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << "\n";
		std::cerr << "initialisation failed\n";
		ret = 1;
	}
	catch(std::string &e) //remplace la string par une vrai exception ça serait un peut mieux
	{
		std::cerr << e;
		ret = 1;
	}
	return ret;
}
