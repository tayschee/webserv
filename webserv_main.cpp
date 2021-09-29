#include "webserv.hpp"

static int check_argument(int c)
{
	int i = 0;

	if (c != 2)
	{
		i = 1;
		std::cout << "Invalid number of argument\n";
	}
	return i;
}

// static int start(cluster &cl)
// {
// 	int ret = 0;
// 	try
// 	{
// 		cl.start();
// 	}
// 	catch(std::string &e) //remplacela string par une vrai exception ça serait un peut mieux
// 	{
// 		std::cerr << e << std::endl;
// 		ret = 1;
// 	}
// 	catch(std::exception &e)
// 	{
// 		std::cerr << e.what();
// 		ret = 1;
// 	}
// 	return ret;
// }

int main(int c, char **v)
{
	int ret = 0;
	int i = check_argument(c);

	if (i)
		return 1;

	try
	{
		cluster cl(v[i + 1]);
		cl.init_listen();
		cl.start();
		//ret = start(cl);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		ret = 1;
	}
	catch(std::string &e) //remplace la string par une vrai exception ça serait un peut mieux
	{
		std::cerr << e << std::endl;
		ret = 1;
	}
	return ret;
}
