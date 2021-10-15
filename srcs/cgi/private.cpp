#include "cgi.hpp"

extern int is_alive;

char            **cgi::init_env(const request &req, const parser &pars, const std::string &path, int &fdin)
{
    std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block(BLOCK_SERVER).conf.find(BLOCK_ROOT)->second[0];
	size_t size = 0;
	struct stat fd_stat;
	if (fstat(fdin, &fd_stat) == 0)
		size = fd_stat.st_size;
	env_tmp[DEF_HTTP_X_SECRET_HEADER_FOR_TEST] = req.get_secret();
	env_tmp[DEF_AUTH_TYPE] = req.get_auth_type();
	env_tmp[DEF_CONTENT_LENGTH] = ft_itoa(size);//req.get_content_length();
	env_tmp[DEF_CONTENT_TYPE] = req.get_content_type();
	env_tmp[DEF_GATEWAY_INTERFACE] = GATEWAY_INTERFACE;
	env_tmp[DEF_PATH_INFO] = req.get_uri();
	env_tmp[DEF_PATH_TRANSLATED] = path;
	env_tmp[DEF_QUERY_STRING] = req.get_query();
	env_tmp[DEF_REMOTE_ADDR] = req.get_host();
	env_tmp[DEF_REMOTE_IDENT] = req.get_user();
	env_tmp[DEF_REMOTE_USER] = req.get_user(); // user id
	env_tmp[DEF_REQUEST_METHOD] = req.get_method();
	env_tmp[DEF_REQUEST_URI] = req.get_uri();
	env_tmp[DEF_SCRIPT_NAME] = pars.get_block(BLOCK_CGI, get_extension(path)).conf.find(SCRIPT_NAME)->second[0];
	env_tmp[DEF_SEVER_NAME] = pars.get_block(BLOCK_SERVER).name;
	env_tmp[DEF_SERVER_PORT] = pars.get_block(BLOCK_SERVER).conf.find(LISTEN)->second[0];
	env_tmp[DEF_SERVER_PROTOCOL] = HTTP_VERSION;
	env_tmp[DEF_SERVER_SOFTWARE] = WEBSERV;

	char	**env = new char*[env_tmp.size() + 1];
	if (!env)
		return NULL;
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_tmp.begin(); it != env_tmp.end(); ++it)
	{
		std::string	element = it->first + "=" + it->second;
		env[j] = new char[element.size() + 1];
		if (!env)
			return NULL;
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
    return env;
}

void			cgi::clear(char **env)
{
	int i = 0;

	if (env)
	{
		while (env[i])
		{
			if (env[i])
			{
				delete[](env[i]);
				env[i] = NULL;
				i++;
			}
		}
		delete[](env);
		env = NULL;
	}
}

void		cgi::son(int &fdin, int &fdout, int save_in, int save_out, const char *script_name, char **env)
{
	char **nll = NULL;
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);

	lseek(fdin, 0, SEEK_SET);
	dup2(fdin, STDIN_FILENO);
	dup2(fdout, STDOUT_FILENO);
	close(fdin);
	close(fdout);

	execve(script_name, nll, env);

	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
	clear(env);
	std::cerr << "Execve crashed." << std::endl;
	throw std::string("quit cgi");
}

void sighandlercgi(const int signal) // catch the signals
{
	if (signal)
		is_alive = 0;
}

std::string     cgi::exec(char **env, const parser &pars, const std::string &path, int &fdin, int &fdout, int fd)
{
    pid_t			pid;
	int				save_in = 0;
	int				save_out = 0;
	std::string		new_body;

	pid = fork();
	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
	}
	else if (pid == 0)
	{
		close(fd);
		fd = -1;
		son(fdin, fdout, save_in, save_out, pars.get_block(BLOCK_CGI, get_extension(path)).conf.find(SCRIPT_NAME)->second[0].c_str(), env);
	}
	else
		my_pid = pid;
	clear(env);
	return new_body;
}