#include "client.hpp"

void                            client::receive() // call recieve
{
    reset_time();
    read = req.receive(fd, rcm);
    std::cout << "read = " << read << std::endl;
}

long                         client::sent() // send response
{
    read = 0;
    response rp(req, pars);
    std::string s(rp.get());
     //std::cout << "=====================================================" << std::endl;
     //std::cout << req.get() << std::endl;
    //   std::cout << "=====================================================" << std::endl;
    //   std::cout << s << std::endl;
    //  std::cout << "=====================================================" << std::endl;
    send(fd, s.c_str(), s.size(), 0);
    return 0;
}