#include "cgi.hpp"

char            **cgi::init_env(const request &req, const parser &pars, const std::string &path)
{
	std::cout << req.get() << std::endl;
    std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block("server").conf.find("root")->second[0];

	std::cout << "QUERY = " << req.get_query() << std::endl;

	env_tmp["AUTH_TYPE"] = req.get_auth_type();
	env_tmp["CONTENT_LENGTH"] = req.get_content_length();
	env_tmp["CONTENT_TYPE"] = req.get_content_type();
	env_tmp["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
	env_tmp["PATH_INFO"] = req.get_uri();
	env_tmp["PATH_TRANSLATED"] = path;
	env_tmp["QUERY_STRING"] = req.get_query();
	env_tmp["REMOTE_ADDR"] = req.get_host();
	env_tmp["REMOTE_IDENT"] = req.get_user();
	env_tmp["REMOTE_USER"] = req.get_user(); // user id
	env_tmp["REQUEST_METHOD"] = req.get_method();
	env_tmp["REQUEST_URI"] = req.get_uri();
	env_tmp["SCRIPT_NAME"] = pars.get_block("cgi", ".php").conf.find("script_name")->second[0];
	env_tmp["SEVER_NAME"] = pars.get_block("server").name;
	env_tmp["SERVER_PORT"] = pars.get_block("server").conf.find("listen")->second[0];
	env_tmp["SERVER_PROTOCOL"] = HTTP_VERSION;
	env_tmp["SERVER_SOFTWARE"] = WEBSERV;

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

void		cgi::son(const int save_in, const int save_out, int fd[2], int fd2[2], const char *script_name, char **env)
{
	char **nll = NULL;

	close(fd2[0]);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd2[1], STDOUT_FILENO);
	execve(script_name, nll, env);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(fd2[1]);
	clear(env);
	std::cerr << "Execve crashed." << std::endl;
	throw std::string("quit");
	close(fd2[1]);
}

void			cgi::father(const int fd[2], std::string &new_body)
{
	close(fd[1]);
	char	buffer[2048];
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

std::string     cgi::exec(char **env, const request &req, const parser &pars)
{
    pid_t			pid;
	int				save_in, save_out;
	std::string		new_body;
	
    // save stdin and stdout
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);

	int fd[2];
	int fd2[2];
	pipe(fd);
	pipe(fd2);

	if (write (fd[1], req.get_body().c_str(), req.get_body().size()) == -1)
		return "500";

	pid = fork();
	if (pid == -1)
	{
        close(fd[0]);
        close(fd[1]);
		std::cerr << "Fork crashed." << std::endl;
	}
	else if (pid == 0)
	{
		son(save_in, save_out, fd, fd2,
		pars.get_block("cgi", ".php").conf.find("script_name")->second[0].c_str(), env);
	}
	else
		father(fd2, new_body);	
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	clear(env);
	return new_body;
}