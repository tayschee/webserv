#ifndef CGI_HPP
# define CGI_HPP

# include <sys/wait.h>
# include <stdio.h>
# include <string.h>
# include  "message/request.hpp"

class cgi
{
    private: /*private function*/
        cgi(const cgi& other);
        cgi &operator=(const cgi& other);
        
        void			        clear(char **env);
        std::string             exec(char **env, const request &req, const parser &pars);
        char                    **init_env(const request &req, const parser &pars, const std::string &path);
        void                    son(const int save_in, const int save_out, int fd[2], int fd2[2], const char *script_name, char **env);
        void                    father(const int fd[2], std::string &new_body);

    public: /*public function*/
        cgi(const request &req, const parser &pars, std::string &body, const std::string &path);
        ~cgi();
};

#endif