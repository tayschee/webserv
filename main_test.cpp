#include "webserv.hpp"

int main(int c, char **v)
{
    if (c != 2)
        return 1;
    std::__1::vector<parser::address_conf, std::__1::allocator<parser::address_conf> > pars_config = parser::parse_folder(v[1]);
    request req("GET / HTTP/1.1\r\nHost: x\r\n\r\n");

    response  resp(req, pars_config.begin());
}