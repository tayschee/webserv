#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <time.h>
# include <sys/time.h>
# include "server.hpp"
# include "cgi.hpp"
# include "message/request.hpp"
# include "message/message.hpp"
# include "message/response.hpp"
# include "parser.hpp"

class client
{
    private: /*private function*/
        int                             fd;
        struct timeval                  time;
        bool                            listen; // is socket for listening or a new client
        int                             my_read;   // finished to read or not 
        int                             nb_pars;
        message::receive                rcv;
        bool                            reset;
	    std::string		                func;
        bool                            alive;
        bool                            first;
        std::string                     &msg;
        std::string                     &resp;
        std::string                     &header;
        int                             fdbody;

        int                             fdin;
        int                             fdout;
	    FILE                            *file_in;
        std::string                     save_path;
        const parser                    *save_pars;
        cgi                             *my_cgi;
        bool                            cgi_start;
        int                             h;

        client();

    public: /*public function*/
		bool			is_close;
        client(const int pfd, const bool _listen, int _nb_pars);
        client(int _fd, bool _listen, const client &other);

        client(const client& other);
        client& operator=(const client& other);
        ~client();

        void                            reset_time(); // Set the time at the current time
        int                             get_fd() const;
        int                             get_fdbody() const;
        int                             get_fdin() const;
        int                             get_fdout() const;
        int                             get_nb_pars() const;
        int                             get_first() const;
        bool                            receive(const fd_set &writefds); // manage receive
        bool                            sent(const std::vector<parser::address_conf> &vec_parser, const fd_set &readfds, const fd_set &writefds); // send response
        bool                            is_listen() const; // Check if listening socket
        bool                            is_time() const; // Check if the time is finished
        int                             is_read() const; // Check if read
        bool                            is_reset() const; // Check if read
		void							reset_rcv(size_t buf_size);
        int			                    add_body();
        void                            parse_body(std::string &request_str, std::string &header);
        void                            error(int status);
        bool                            select_method(const fd_set &readfds, const fd_set &writefds);
        bool                            add_body(const fd_set &readfds, const fd_set &writefds);
        bool                            put(const fd_set &readfds, const fd_set &writefds);
        bool                            method_cgi(const fd_set &readfds, const fd_set &writefds);
        bool	                        send_header();
        void                            del();
};

#endif