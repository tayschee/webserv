#include "client.hpp"

void                            client::receive() // call recieve
{
    reset_time();
    if ((read = req.receive(fd, rcm)) > 0)
        rep = response(req, pars);

   // std::cout << "read = " << read << std::endl;
}

long                         client::sent() // send response
{
    //read = 0;
    //response rp(req, pars);
    // std::cout << "=====================================================" << std::endl;
	// //std::cout << "allow = " << pars.get_block("server").conf.find("allow")->second << std::endl;
    // std::cout << req.get() << std::endl;
    read = rep.sent(fd);
    //td::cout << "=====================================================3" << std::endl;
    return read;
}