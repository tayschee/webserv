#include "cgi.hpp"

char            **cgi::init_env(const request &req, const parser &pars, const std::string &path)
{
    std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block("server").conf.find("root")->second[0];

	env_tmp["AUTH_TYPE"] = ""; // idendify type
	env_tmp["CONTENT_LENGTH"] = ft_itoa(path.size());
	env_tmp["CONTENT_TYPE"] = "text/html";
	env_tmp["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_tmp["PATH_INFO"] = req.get_uri();
	env_tmp["PATH_TRANSLATED"] = path;
	env_tmp["QUERY_STRING"] = req.get_uri();
	env_tmp["REMOTE_ADDR"] = ""; // client's IP
	env_tmp["REMOTE_IDENT"] = ""; //distance user
	env_tmp["REMOTE_USER"] = ""; // user id
	env_tmp["REQUEST_METHOD"] = req.get_method();
	env_tmp["REQUEST_URI"] = req.get_uri();
	env_tmp["SCRIPT_NAME"] = pars.get_block("cgi", ".php").conf.find("script_name")->second[0];
	env_tmp["SEVER_NAME"] = pars.get_block("server").name;
	env_tmp["SERVER_PORT"] = pars.get_block("server").conf.find("listen")->second[0];
	env_tmp["SERVER_PROTOCOL"] = req.get_version();
	env_tmp["SERVER_SOFTWARE"] = "WEBSERV 1.0";

	char	**env = new char*[env_tmp.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_tmp.begin(); it != env_tmp.end(); ++it)
	{
		std::string	element = it->first + "=" + it->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
    return env;
}

void			cgi::clear(char **env)
{
	int i = 0;
	while (env[i])
		delete[](env[i++]);
	delete[](env);
}

void		cgi::son(const int save_in, const int save_out, int fd[2], const char *script_name, char **env)
{
	char **nll = NULL;
	close(fd[0]);
	dup2(fd[1], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	execve(script_name, nll, env);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(fd[1]);
	clear(env);
	std::cerr << "Execve crashed." << std::endl;
	throw std::string("quit");
}

void			cgi::father(const int fd[2], std::string &new_body)
{
	char	buffer[2048];
	close(fd[1]);
	int ret = 1;
	while (ret > 0)
	{
		memset(buffer, 0, 2048);
		ret = read(fd[0], buffer, 2048 - 1);
		new_body += buffer;
	}
	if (!new_body.size())
		new_body = "500";
	close(fd[0]);
}

std::string     cgi::exec(char **env, const parser &pars)
{
    pid_t			pid;
	int				save_in, save_out;
	std::string		new_body;
	
    // save stdin and stdout
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	int fd[2];
	pipe(fd);

	pid = fork();
	if (pid == -1)
	{
        close(fd[0]);
        close(fd[1]);
		std::cerr << "Fork crashed." << std::endl;
	}
	else if (pid == 0)
	{
		son(save_in, save_out, fd,
		pars.get_block("cgi", ".php").conf.find("script_name")->second[0].c_str(), env);
	}
	else
		father(fd, new_body);	
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	clear(env);
	return new_body;
}