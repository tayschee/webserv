#include <message/response.hpp>

/*Convert header_type to a syntax adapt for http do same thing than (std::string std::string::operator=(const header_type &)),
that doesn't exist for now, but it can be implement*/
std::string		response::header_to_string() const
{
	const_iterator it(header.begin());
	const_iterator end(header.end());
	std::string str;

	while (it != end)
	{
		str += it->first;
		str += ": ";
		str += it->second;
		str += CRLF;
		++it;
	}
	return str;
}

/*add inside response:header all field which are in all method and in all condition*/
void			response::main_header(const std::vector<std::string> &allow_method)
{
	add_allow(allow_method); // allow_field add in header
	add_date(); //date field add in header
	add_server(); //server field add in header
}

/*create first line of response header */ 
std::string		response::header_first_line() const
{
	std::string		str_first_line;

	str_first_line = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.version + CRLF;

	return str_first_line;
}

//this function van be put in utils.hpp, it gives list of files inside directory
std::list<std::string>	response::files_in_dir(const std::string &path) const
{
	DIR *directory = opendir(path.c_str());
	struct dirent *entry;
	std::list<std::string> files;

	if (directory == NULL)
	{
		//do something
	}
	while ((entry = readdir(directory)))
	{
		if (entry->d_type == DT_REG || entry->d_type == DT_LNK) //if this is s file
			files.push_back(entry->d_name);
	}
	closedir(directory);
	return files;
}