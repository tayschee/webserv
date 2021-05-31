#include "client.hpp"

void                            client::receive() // call recieve
{
    reset_time();
    read = rcm.receive(fd);
	//read == request::receive(fd, rcm);
	
	while ((read = rcm.check())) //check dest_fd that could be a request
	{
		int buf = 10; // do a function
		request	req(rcm.get_msg().c_str());
		response resp(req, pars);

		sdm.add_element_to_send(resp, buf);
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