#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "parser.hpp"
# include "server.hpp"
# include "client.hpp"
# include "message/response.hpp"
# include <csignal>
# include <list>

class cluster
{
    public:
        typedef std::list<client>::iterator iterator;

    private: /*private function*/
        std::list<client>       list_client; // list all clients

        cluster();
        cluster(const cluster& other); // cannot copy cluster
        cluster& operator=(const cluster& other); // cannot copy cluster

        void	                close_client(iterator &it); // close a client
        void	                set_list_fd(fd_set &readfds, fd_set &writefds, int &max); // initialize all sockets
        int                     wait_activity(fd_set &readfds, fd_set &writefds); // wait a activity as read or write
        int 	                receive(client &cl, const int &fd, iterator &it); // call receive
        int		                send_response(client &cl); // send to response

    public: /*public function*/
        cluster(const std::string _path); // constructor used
        ~cluster(); // destructor

        int		                init_listen(); // initializing listens
        int                     start(); // start mananging sockets
};

#endif