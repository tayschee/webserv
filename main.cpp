#include "cluster.hpp"
#include "message.hpp"

int		main()
{
	response	resp(503);
	
	std::cout << resp.get() << "\n";
	return 0;
}
