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
        std::string             exec(char **env, const request &req, const parser &pars, const std::string &path);
        char                    **init_env(const request &req, const parser &pars, const std::string &path);
        void                    son(long fdin, long fdout, const char *script_name, char **env);
        void			        father(long fdout, std::string &new_body);


    public: /*public function*/
        cgi(const request &req, const parser &pars, std::string &body, const std::string &path);
        ~cgi();
};

#endif