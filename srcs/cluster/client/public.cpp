#include "client.hpp"

void                            client::receive() // call recieve
{
	int i;
	std::string msg;

    reset_time();
    i = rcv();
	if (i == -1)
	{
		read = -1;
	}
	else
	{
		if (rcv.check() | rcv.BODY_MASK)
		{
			msg = rcv.get_buffer();
			rcv.prepare_next();
			read = 1;
			req = request(msg.c_str());
		}
	}
}

void							client::reset_rcv(size_t buf_size)
{
	rcv.reset(fd, buf_size);
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