#include "client.hpp"

void                            client::receive() // call recieve
{
    reset_time();
    read = req.receive(fd, rcm);
}

ssize_t                         client::sent() // send response
{
    read = false;
    response rp(req, pars);
//    std::cout << rp.get() << std::endl;
    return send(fd, rp.get().c_str(), rp.get().size(), 0);
}