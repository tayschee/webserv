Syntax description:

	- One property per line.
	- The name of the file must have the conf extension to be seen by the parser (other files will be ignored).
	- Each line begins with the property's name and is followed by arguments space-separated.
	- Each property is unique within a given block. Only the last property will be kept.
	- Each block is delimited by {}. A block is determined by its name and arguments. Example:

name arg0 arg1 ... argn {
	instructions
}

	- The block's first line must end with a { (If it's in the beginning of the next line, it will be an error, since a block need a name and arguments)
	- The block's last line must contain only a } to close the block. The } cannot be on the last property's line (it will be interpreted as part of the property).
	- The whole file represents a single block that is named server.

Main blocks and instructions:

	- location blocks allows us to configure a specific path of URI. They are written with an only argument that is the same path as in the URI.
	- cgi blocks allows the configuration of CGI in the server. They are written with the needed extension, you can put it with or without the point prefix. Example : if you put "cgi .php" or "cgi php" it will be the same result.
	- error_page and location_error should be used together:
		- error_page is an property that indicates which errors are treated with a specific file. It should be only in the server block (ie in no apparent block).
		- location_error is a block that specifies where are the errors specified in error_page. It is written with an argument which specifies the concerned group of errors, the variable numbers are replaced with an 'x'. These codes should be terminated with the extension wanted. Example: location_error 40x.html.
		If there is no location_error, the location of all the specified errors (with error_page) will fallback to "location /".
		If there is no error_page, it must not be any location_error block. This will result to an error.

	- The "location /" block is mandatory. Not putting it will result in an error and the removal of the file from the parsing architecture.
