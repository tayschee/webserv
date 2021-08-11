#include "parser.hpp"

void append_buffer(std::string &buffer, std::string &line, size_t limit, bool resize)
{
	line += buffer.substr(0, limit);

	if (resize)
	{
		buffer.erase(0, limit + 1);
		buffer.resize(BUFFER_SIZE, '\0');
	}
	else
		buffer.assign(BUFFER_SIZE, '\0');
}

bool parser::getline(int fd, std::string &line)
{
	line.clear();
	ssize_t i = 0;
	std::string::size_type limit;

	if ((limit = buffer.find('\n')) != buffer.npos)
	{
		append_buffer(buffer, line, limit, true);
		return true;
	}
	else if (buffer[0] != '\0')
		append_buffer(buffer, line, buffer.find('\0'), false);
	while ((i = read(fd, &buffer[0], BUFFER_SIZE)) > 0 &&
		   (limit = buffer.find('\n')) == buffer.npos)
		append_buffer(buffer, line, limit, false);
	if (i < 0)
		return false;
	append_buffer(buffer, line, limit, true);
	return i > 0;
}

std::string parser::find_best_match(std::string arg) const
{
	if (arg.empty() || arg[0] != '/')
		arg.insert(arg.begin(), '/');

	blocks::key_type key = std::make_pair(PARSER_LOCATION, std::vector<std::string>(1, arg));

	if (_blocks.find(key) != _blocks.end())
		return arg;

	while (!arg.empty())
	{
		key.second[0] = arg;
		if (_blocks.find(key) != _blocks.end() || arg == "/")
			return arg;
		arg.erase(arg.rfind('/'));
	}
	return "/";
}

std::string parser::remove_comments(const std::string &line)
{
	std::string::size_type pos = line.find('#');

	return pos == line.npos ? line : line.substr(0, pos);
}

std::string parser::chk_prefix(const std::string &str, char pref)
{
	if (str[0] == pref)
		return str;
	return pref + str;
}
