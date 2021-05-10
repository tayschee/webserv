#include "cgi.hpp"

cgi::cgi(const request &req, const parser &pars, std::string &body)
{
	body = exec(init_env(req, pars), pars);

		if (body == "error" || body[0] == '5')
			return;

		//std::string cmp("cgi");
		//std::string resp_str;
		//const_iterator	it(header.begin());
		//const_iterator	end(header.end());
		int i = 0;
		std::string tmp;
		while (body[i] != '<')
				i++;
	/*	header["Content-Length"] =  ft_itoa(body.size() - i);
		header["Content-Type"] =  "text/html";

		std::vector<std::string> tab;
		std::vector<std::string> tab2;
		
		tab = split(tmp.c_str(), "\n\t");
		for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end() && it != --tab.end(); ++it)
		{
			tab2 = split(*it, ":");
			header[tab2[0]] = tab2[1];
		}*/

		body = body.c_str() + i;
}

cgi::~cgi(){}