#include "cgi.hpp"

cgi::cgi(const request &req, const parser &pars, std::string &body, const std::string &path)
{
	body = exec(init_env(req, pars, path), req, pars);
	if (body[0] == '5')
		return;

		//std::string cmp("cgi");
		//std::string resp_str;
		//const_iterator	it(header.begin());
		//const_iterator	end(header.end());
		unsigned long i = 0;
	//	std::string tmp;
		bool crl = false;
		while (i < body.size())
		{
			if (body[i] == '\r' || body[i] == '\n')
			{
				if (crl == true)
				{
					i++;
					break;
				}
				else
					crl = true;
			}
			else
				crl = false;
			i++;
		}
		if (i == body.size())
			i = 0;
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