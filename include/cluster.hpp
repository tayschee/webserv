#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "server.hpp"
#include "message/message.hpp"
#include "message/request.hpp"
#include "message/response.hpp"
#include "parser.hpp"

class cluster
{
    private: /*private function*/
    std::list<client> 			list_client;
	std::vector<server> 		tab;
	std::vector<parser> 		pars;

    public: /*public function*/
       // cluster(const cluster& other); // cannot copy cluster
	    typedef std::list<client>::iterator iterator;

        cluster();
        cluster(const std::string _path);
        ~cluster();

    int		init_listen();
    int     start();
    void	close_client(iterator &it);

     //   cluster& operator=(const cluster& other); // cannot copy cluster

       
};

#endif