#include "message/response.hpp"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h> 

int		response::method_is_head(const request &req, const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (lstat(file.c_str(), &file_stat) < 0)
	{
		return (error_file(errno));
	}

	/* two next line can maybe be add to main_header */
	add_content_length(file_stat.st_size); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file, req);

	return 200; //value of OK response
}

std::string		index(std::string path)
{
    errno = 0;
	std::cout << "path = " << path << std::endl;
	DIR *dir = opendir(path.c_str());\
	struct dirent *dp;
	std::string index =\
	"<html>\n\
	<head><title>Index of /</title></head>\n\
	<body>\n\
		<h2>Index of /</h2>\n\
		<ui>\n\
		<ur>";\
			while ((dp = readdir(dir)) != NULL)
			{
				index += "<li><a href=\"" + std::string(dp->d_name) + "\">" + std::string(dp->d_name) + "</a></li>\n";
			}
			if (errno != 0)
			{
				std::cout << "xxxxxxxxxxxxxxxxxxxx" << std::endl;

			}
            closedir(dir);

	index +=\
		"<ui>\n\
	</body>\n\
	</html>";
	return index;
}

std::string		cgi(const request &req, const parser &pars)
{
	pid_t			pid;
	int				save_in;
	int				save_out;
	std::string		new_body;


	std::map<std::string, std::string> env_tmp;
	std::string root = pars.get_block("server").conf.find("root")->second[0];
	std::string path(root + "/" + req.get_uri());

	env_tmp["AUTH_TYPE"] = ""; // idendify type
	env_tmp["CONTENT_LENGTH"] = "";
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
	for (std::map<std::string, std::string>::const_iterator i = env_tmp.begin(); i != env_tmp.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;

	// save stdin and stdout
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);

	int		ret = 1;

	int fd[2];
	pipe(fd);

	pid = fork();

	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		//return un erreur 500
	}
	else if (!pid)
	{
		char **nll = NULL;
		close(fd[0]);
		dup2(fd[1], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		
		execve(pars.get_block("cgi", ".php").conf.find("script_name")->second[0].c_str(), nll, env);
		std::cerr << "Execve crashed." << std::endl;
		//return un erreur 500;
	}
	else
	{
		char	buffer[2048] = {0};
		close(fd[1]);
		lseek(fd[0], 0, SEEK_SET);
		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 2048);
			ret = read(fd[0], buffer, 2048 - 1);
			new_body += buffer;
		}
		waitpid(-1, NULL, 0);
		close(fd[0]);
	}

	int i = 0;
	while (env[i])
		delete[](env[i++]);
	delete[](env);
	dup2(STDIN_FILENO, save_in);
	dup2(STDOUT_FILENO, save_out);

	return new_body;
}

int		response::method_is_get(const request &req, const parser &pars)
{
	int		fd = -1;
	struct stat file_stat; //information about file

	std::string	file = pars.get_block("server").conf.find("root")->second[0] + req.get_uri();

	//403 interdiction
	
	std::cout << "file = " << file << std::endl;
	if (lstat(file.c_str(), &file_stat) < 0)
		return (error_file(errno));
	std::string CGI(req.get_uri());
	std::string::iterator it = CGI.end();
	while (--it != CGI.begin())
	{
		if (*it == '.')
			break;
	}
	std::string cmp(it, CGI.end());
	if (cmp == ".php")
	{
		body = cgi(req, pars);
		header.insert(value_type("cgi", "true"));
		std::string cmp("cgi");
		std::string resp_str;
		const_iterator	it(header.begin());
		const_iterator	end(header.end());
		
		int i = 0;
		char tmp[100];
		while (body[i] != '<')
		{
				tmp[i] = body[i];
				i++;
		}
		tmp[i] = '\0';
		header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size() - i)));
		header.insert(value_type(CONTENT_TYPE, "text/html"));

		std::vector<std::string> tab;
		std::vector<std::string> tab2;
		
		tab = split(tmp, "\n\t");
		for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end() && it != --tab.end(); ++it)
		{
			tab2 = split(*it, ":");
			header.insert(value_type(tab2[0], tab2[1]));
		}

		body = body.c_str() + i;
	}
	else if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
	{
		body = index(file);
		header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size())));
		header.insert(value_type(CONTENT_TYPE, "text/html"));
	}
	else 
	{
		if ((fd = open(file.c_str(), O_RDONLY)) < 0)
		{
			return (error_file(errno));
		}
		if (lstat(file.c_str(), &file_stat) < 0)
			return (error_file(errno));
		if (add_body(fd, file_stat)) /*body is filled by content of fd*/
		{
			close(fd);
			return error_file(errno);
		}
		/* almost same than method_is_head but we dont call it because we need struct stat to fill body */
		//add_content_length(req.get_header(), file_stat.st_size); /* st_size = total size in byte */
		header.insert(value_type(CONTENT_LENGTH, ft_itoa(body.size())));

		add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
		add_content_type(file, req);
		if (cmp == ".jpg")
			header.insert(value_type(CONTENT_TYPE, "image/jpeg"));

	}
	if (fd > 0)
		close(fd);
	return 200;
}
/*
int		response::method_is_get(const request &req)
{
	int		fd;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	//403 interdiction
	if ((fd = open(file.c_str(), O_RDONLY)) < 0)
	{
		return (error_file(errno));
	}
	if (lstat(file.c_str(), &file_stat) < 0)
		return (error_file(errno));

	if (add_body(fd, file_stat)) //body is filled by content of fd//
	{
		close(fd);
		return error_file(errno);
	}
	close(fd);

	// almost same than method_is_head but we dont call it because we need struct stat to fill body //
	add_content_length(req.get_header(), file_stat.st_size); // st_size = total size in byte //
	add_last_modified(file_stat.st_mtime); // st_mtime = hour of last modification //
	add_content_type(file);

	return 200;
}*/

int		response::method_is_delete(const request &req, const parser &pars)
{
	(void)pars;
	std::string	file = req.get_uri();

	//check if path is valid

	if (unlink(file.c_str())) //delete the file, if there is a fd associted whith this file, deleted it when the fd is close
	{
		return error_file(errno); //check errno
	}

	return 204;
}

int		response::method_is_options(const request &req, const parser &pars)
{
	(void)pars;
	struct stat file_stat; //information about file
	std::string	file = req.get_uri();

	if (stat(file.c_str(), &file_stat) < 0)
		return error_file(errno); //check errno

	/* two next line can maybe be add to main_header */
	add_content_length(0); /* st_size = total size in byte */
	add_last_modified(file_stat.st_mtime); /* st_mtime = hour of last modification */
	add_content_type(file, req);

	return 200; //value of OK response
}

int		response::method_is_put(const request &req, const parser &pars)
{
	(void)pars;
	int		fd;
	int		response_value = 204;
	std::string	file = req.get_uri();
	std::string body = req.get_body();

	/*verify if content exist*/
	if ((fd = open(file.c_str(), O_WRONLY)) < 0)
	{
		response_value = 201; //CREATE
		if ((fd = open(file.c_str(), O_WRONLY | O_CREAT)) < 0) //content doesn't exist so create it
		{	
			return (error_file(errno));
		}
		/* this header field are specific if file didn't exists */
		add_content_length(0); //Content-length is for size of body and there is no body
		//must add location
	}
	if (write(fd, body.c_str(), body.size()) < 0)
		return 500;
	close(fd);

	add_content_type(file, req);

	return response_value;
}

int			response::method_is_unknow(const request &req, const parser &pars)
{
	(void)pars;
	(void)req;

	return 405; //Method Not Allowed
}