#include "cgi.hpp"

char            **cgi::init_env(const request &req, const parser &pars, const std::string &path)
{
    std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block("server").conf.find("root")->second[0];

	env_tmp["HTTP_X_SECRET_HEADER_FOR_TEST"] = "1";
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
	env_tmp["SCRIPT_NAME"] = pars.get_block("cgi", get_extension(path)).conf.find("script_name")->second[0];
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

void		cgi::son(long fdin, long fdout, const char *script_name, char **env)
{
	char **nll = NULL;
	dup2(fdout, STDOUT_FILENO);
	dup2(fdin, STDIN_FILENO);
	execve(script_name, nll, env);
	close(fdin);
	close(fdout);
	clear(env);
	std::cerr << "Execve crashed." << std::endl;
	throw std::string("quit");
}

void			cgi::father(long fdout, std::string &new_body)
{
	waitpid(-1, NULL, 0);
	lseek(fdout, 0, SEEK_SET);
	char	buffer[2048];
	int ret = 1;
	while (ret > 0)
	{
		memset(buffer, 0, 2048);
		ret = read(fdout, buffer, 2048 - 1);
		new_body += buffer;
	}
	if (!new_body.size())
		new_body = "500";
	close(fdout);
}

std::string     cgi::exec(char **env, const request &req, const parser &pars, const std::string &path)
{
    pid_t			pid;
	int				save_in, save_out;
	std::string		new_body;
	
    // save stdin and stdout
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);

	FILE* file_in = tmpfile();
	FILE* file_out = tmpfile();
	
	long fdin;
	long fdout;

	fdin = fileno(file_in);
	fdout = fileno(file_out);
	if (req.get_body().size() > 0)
	{
		write(fdin, req.get_body().c_str(), req.get_body().size());
		lseek(fdin, 0, SEEK_SET);
	}
	else
		close(fdin);
	std::cout << "start cgi" << std::endl;

	pid = fork();
	if (pid == -1)
	{
		close(fdin);
		close(fdout);
		std::cerr << "Fork crashed." << std::endl;
	}
	else if (pid == 0)
	{
		son(fdin, fdout, pars.get_block("cgi", get_extension(path)).conf.find("script_name")->second[0].c_str(), env);
	}
	else
	{
		close(fdin);
		father(fdout, new_body);
	}
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	clear(env);
	return new_body;
}