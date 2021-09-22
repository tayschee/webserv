#include "cgi.hpp"

char            **cgi::init_env(const request &req, const parser &pars, const std::string &path)
{
    std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block(BLOCK_SERVER).conf.find(BLOCK_ROOT)->second[0];
	env_tmp[DEF_HTTP_X_SECRET_HEADER_FOR_TEST] = req.get_secret();
	env_tmp[DEF_AUTH_TYPE] = req.get_auth_type();
	env_tmp[DEF_CONTENT_LENGTH] = req.get_content_length();
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

	if (env)
	{
		while (env[i])
			delete[](env[i++]);
		delete[](env);
		env = NULL;
	}
}

void		cgi::son(long fdin, long fdout, FILE* file_in, FILE* file_out, int save_in, int save_out, const char *script_name, char **env)
{

	char **nll = NULL;
	long		fderr;
	
	FILE* file_err = tmpfile();
	fderr = fileno(file_err);
	dup2(fdout, STDOUT_FILENO);
	dup2(fdin, STDIN_FILENO);
	dup2(fderr, STDERR_FILENO);
	execve(script_name, nll, env);
	close(fdin);
	fclose(file_in);
	fclose(file_out);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
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
	int				save_in, save_out, save_err;
	std::string		new_body;

    // save stdin and stdout
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	save_err = dup(STDERR_FILENO);

	FILE* file_in = tmpfile();
	FILE* file_out = tmpfile();
	
	long fdin;
	long fdout;

	fdin = fileno(file_in);
	fdout = fileno(file_out);
	if (req.get_body().size() > 0)
	{
		if (write(fdin, req.get_body().c_str(), req.get_body().size()) < 0)
		{
			close(fdin);
			fclose(file_in);
			fclose(file_out);
			dup2(save_in, STDIN_FILENO);
			dup2(save_out, STDOUT_FILENO);
			close(save_in);
			close(save_out);
			clear(env);
			return "500";
		}
		lseek(fdin, 0, SEEK_SET);
	}

	pid = fork();
	if (pid == -1)
	{
		close(fdout);
		std::cerr << "Fork crashed." << std::endl;
	}
	else if (pid == 0)
	{
		son(fdin, fdout, file_in, file_out, save_in, save_out, pars.get_block(BLOCK_CGI, get_extension(path)).conf.find(SCRIPT_NAME)->second[0].c_str(), env);
	}
	else
	{
		father(fdout, new_body);
	}
	close(fdin);
	fclose(file_in);
	fclose(file_out);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	dup2(save_err, STDERR_FILENO);
	close(save_in);
	close(save_out);
	clear(env);
	return new_body;
}