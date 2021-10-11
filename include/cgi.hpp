#ifndef CGI_HPP
# define CGI_HPP

# include <sys/wait.h>
# include <stdio.h>
# include <string.h>
# include  "message/request.hpp"

class cgi
{
    private: /*private function*/
        int                    my_pid;
        cgi(const cgi& other);
        cgi &operator=(const cgi& other);
        
        void			        clear(char **env);
        std::string             exec(char **env, const parser &pars, const std::string &path, int &fdin, int &fdout);
        char                    **init_env(const request &req, const parser &pars, const std::string &path, int &fdin);
        void                    son(int &fdin, int &fdout, int save_in, int save_out, const char *script_name, char **env);
        // void			        father(long fdout, std::string &new_body);


    public: /*public function*/
        int                     get_pid();
        cgi(const request &req, const parser &pars, const std::string &path, int &fdin, int &fdout);
        ~cgi();
};

#endif