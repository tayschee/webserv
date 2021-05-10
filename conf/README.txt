Syntax description:

	- One property per line.
	- The name of the file must have the conf extension to be seen by the parser (other files will be ignored).
	- A comment is specified by a '#' which can be at the beginning or in the middle of the line. Everything after the '#' will be ignored.
	- Each line begins with the property's name and is followed by arguments space-separated.
	- Each property is unique within a given block. Only the last property will be kept.
	- Each block is delimited by {}. A block is determined by its name and arguments. Example:

name arg0 arg1 ... argn {
	properties
}

	- The block's first line must end with a { (If it's in the beginning of the next line, it will be an error, since a block need a name and arguments)
	- The block's last line must contain only a } to close the block. The } cannot be on the last property's line (it will be interpreted as part of the property).
	- The whole file represents a single block that is named server.
	- Blocks cannot contain blocks.

Main blocks and properties:

	- location blocks allows us to configure a specific path of URI. They are written with an only argument that is the same path as in the URI.
	- cgi blocks allows the configuration of CGI in the server. They are written with the needed extension, you can put it with or without the point prefix. Example : if you put "cgi .php" or "cgi php" it will be the same result.
	- error_page binds an error code to an actual html page. It is the only property that can be repeated in a single block. Each error_page takes exactly two parameters that is, in this order, the error code and the associated page. This property will be always related to its parent. Example: error_page 404 404.html.
	- root properties should be used only in location and server blocks. It says to webserv where to search for files with the URI specified by its parent. It takes one and only one argument.
	- index properties should only be located in location blocks. It is meant to be used in conjunction with root, to tell what is the default file to look for, when the URI specifies a folder instead of a file. It can take one or more arguments, and each one of them will be tested until we find an index file in the root folder specified.
	- server_name allows users to specify an IPv4 address (dotted-decimal notation only). It must be outside of any block and it takes only one argument which is the wanted address.
	- listen specifies a port to listen on. It takes exactly one argument which is an integer between 1 and 65535. As the server_name property, it must be outside of any block and must take only the port as argument.

	- The "location /" block is mandatory. Not putting it will result in an error and the removal of the file from the parsing architecture.

	- If no server_name is specified, the default IP adress shall be 127.0.0.1.
	- If no listen is specified, the default port will be 80.
	- In the "location /", it must be at least a root and an index properties.

Other blocks and properties: (please add every block/instrution that I may have forgotten)

	- return allow users to perform redirections. It takes 2 arguments, the redirection code and the target, in this order.
	- accept property provides a list of accepted methods for its parent (can be in the server block, it specifies a list of accepted methods for the whole server, and can be overwritten by location blocks). It may accept several arguments, and each one of them must be an existing method (see RFC to know what is implemented).
	- autoindex specifies if a location without index file should return an error 403 or not. It can only be in location blocks and can only take one argument that is either on or off (by default, autoindex is off). If the autoindex is off and the current folder do not have an index file, it will return an error. However, if the autoindex is on, then the server will generate a default view of servers.
