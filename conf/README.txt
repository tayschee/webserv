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
	- error_page and location_error should be used together:
		- error_page is an property that indicates which errors are treated with a specific file. It should be only in the server block (ie in no apparent block).
		- location_error is a block that specifies where are the errors specified in error_page. It is written with an argument which specifies the concerned group of errors, the variable numbers are replaced with an 'x'. These codes should be terminated with the extension wanted. Example: location_error 40x.html.
		If there is no location_error, the location of all the specified errors (with error_page) will fallback to "location /".
		If there is no error_page, it must not be any location_error block. This will result to an error.
	- root properties should be used only in location and location_error blocks. It says to webserv where to search for files with the URI specified by its parent. It takes one and only one argument.
	- index properties should only be located in location blocks. It is meant to be used in conjunction with root, to tell what is the default file to look for, when the URI specifies a folder instead of a file. It can take one or more arguments, and each one of them will be tested until we find an index file in the root folder specified.
	- listen specifies a port to listen on IP and a port. The IP must be an IPv4. Port and IP address are separated by a ':' and no space (Example: listen 127.0.0.1:80).

	- The "location /" block is mandatory. Not putting it will result in an error and the removal of the file from the parsing architecture.

	- If no listen is specified, the default address will be 127.0.0.1:80.
	- In the "location /", it must be at least a root and an index properties.

Other blocks and properties: (please add every block/instrution that I may have forgotten)

	- server_name property allow users to give a name to a virtual server. This name can be used to redirect to do internal redirections. It takes only one argument that is the wanted name.
	- return allow users to perform redirections. It takes 2 arguments, the redirection code and the target, in this order.
	- accept property provides a list of accepted methods for its parent (can be in the server block, it specifies a list of accepted methods for the whole server, and can be overwritten by location blocks). It may accept several arguments, and each one of them must be an existing method (see RFC to know what is implemented).
