#include "parser.hpp"

parser::block	parser::create_default_mime_type()
{
	block	default_mime_type(PARSER_TYPES);

	default_mime_type.conf.insert(std::make_pair(".html", std::vector<std::string>(1, "text/html")));
	default_mime_type.conf.insert(std::make_pair(".htm", std::vector<std::string>(1, "text/html")));
	default_mime_type.conf.insert(std::make_pair(".bmp", std::vector<std::string>(1, "image/x-ms-bmp")));
	default_mime_type.conf.insert(std::make_pair(".jpg", std::vector<std::string>(1, "image/jpeg")));
	default_mime_type.conf.insert(std::make_pair(".png", std::vector<std::string>(1, "image/png")));

	return default_mime_type;
}

void parser::parse_mime(const std::string& filename)
{
	buffer.assign(BUFFER_SIZE, '\0');

	int file = open(filename.c_str(), O_RDONLY); //WARNING
	std::string line;
	int line_no = 0;

	if (file == -1)
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return;
	}

	while (getline(file, line))
	{
		line_no++;
		if (line.empty())
			continue;
		line = remove_comments(line);
		if (line.empty())
			continue;
		line = clean_string(line);
		if (line.empty())
			continue;
		if (line.find('{') != line.npos ||
			line.find('}') != line.npos)
			continue; // Lines delimitting a block are ignored (to simplify how the file is written)
		parse_line_mime(filename, line, line_no);
	}

	close(file);
	buffer.clear();
}

void parser::parse_line_mime(const std::string &filename, const std::string& line, int line_no)
{
	std::vector<std::string> splitted = split(line);

	if (splitted.size() != 2)
	{
		std::cerr << "Error: " << filename << ": Mime requires exactly 2 arguments which is, in this order, extension and type. (line: " << line_no << ")\n";
		return ;
	}

	mime.conf[splitted[0]] = std::vector<std::string>(1, splitted[1]);
}
