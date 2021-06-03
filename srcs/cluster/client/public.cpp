#include "client.hpp"

void                            client::receive() // call recieve
{
	int i;

    reset_time();
    i = rcm.receive(fd);

	while (i == 1)
	{
		std::string message(rcm.get_data());
	}
	// req = rcm.get_request()

}

void							client::reset_rcm(size_t buf_size)
{
	rcm.reset(buf_size);
}

long                         client::sent() // send response
{
    read = 0;
    response rp(req, pars);
    // std::cout << "=====================================================" << std::endl;
	// //std::cout << "allow = " << pars.get_block("server").conf.find("allow")->second << std::endl;
    // std::cout << req.get() << std::endl;
    //  std::cout << "=====================================================" << std::endl;
    std::string s(rp.get());
    //    std::cout << s << std::endl;
    // std::cout << "=====================================================" << std::endl;
	std::cout << "\n-------------------------------\n" << s << "\n-------------------------------\n";
    send(fd, s.c_str(), s.size(), 0);
    return 0;
}