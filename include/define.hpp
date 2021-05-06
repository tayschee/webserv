#ifndef DEFINE_HPP
# define DEFINE_HPP

/*Server*/
# define WEBSERV "webserv"
# define HTTP_VERSION "HTTP/1.1"

/*line return*/
# define CRLF "\r\n"
# define SEPARATOR CRLF CRLF

# define ERROR_FILE_NAME ".html" /*to use a funtion simply*/

# define ERROR_FILE		"<html>\n"\
						"<head><title>xxx</title></head>\n"\
						"<body>\n"\
						"<center><h1>xxx</h1></center>\n"\
						"<hr><center>webserv</center>\n"\
						"</body>\n"\
						"</html>"

/*parser block*/
# define BLOCK_SERVER "server"
# define BLOCK_LOCATION "location"
# define BLOCK_ERROR "location_error"
# define BLOCK_CGI "cgi"

/*parser variable*/
# define ACCEPT "accept"

/*define header field*/
# define ALLOW "Allow"
# define ACCEPT_CHARSET "Accept-Charset"
# define ACCEPT_LANGUAGE "Accept-Language"
# define AUTHORIZATION "Authorization"
# define CONTENT_LENGTH "Content-Length"
# define REFERER "Referer"
# define USER_AGENT "User-Agent"
# define HOST "Host"
# define LAST_MODIFIED "Last-Modified"
# define SERVER	"Server"
# define LOCATION "Location"
# define RETRY_AFTER "Retry-After"
# define CONTENT_TYPE "Content-Type"
# define DATE "Date"
# define LOCATION "Location"
# define TRANSFERT_ENCODING "Transfert-Encoding"
# define CONTENT_LANGUAGE "Content-Language"
# define WWW_AUTHENTIFICATE "WWW-Authentificate"

/*define methods*/
# define GET "GET"
# define HEAD "HEAD"
# define POST "POST"
# define PUT "PUT"
# define DELETE "DELETE"
# define CONNECT "CONNECT"
# define OPTIONS "OPTIONS"
# define TRACE "TRACE"
# define PATCH "PATCH"

/*define status (please sort them)*/
# define STATUS_200 "OK"
# define STATUS_201 "Created"
# define STATUS_204 "No Content"
# define STATUS_301 "Moved Permanently"
# define STATUS_304 "Found" //Nor modified ?
# define STATUS_400 "Bad Request"
# define STATUS_401 "Unauthorized"
# define STATUS_403 "Forbidden"
# define STATUS_404 "Not Found"
# define STATUS_405 "Method Not Allowed"
# define STATUS_406 "Not Acceptable"
# define STATUS_409 "Conflict"
# define STATUS_501 "Not Implemented"
# define STATUS_503 "Service Unavailable"
# define UNKNOW_STATUS "Unknow"

/*define type*/
# define APP		"application/"
# define AUDIO		"audio/"
# define EXEMPLE	"exemple/"
# define FONT 		"font/"
# define IMAGE		"image/"
# define MODEL		"model/"
# define TXT		"text/"
# define VIDEO		"video/"

# define DEFAULT_TYPE "text/"

/*define subtype*/

# define BMP	"bmp"
# define CSS 	"css"
# define HTML	"html"
# define GIF	"gif"
# define JPEG	"jpeg"
# define JS		"javascript"
# define OGG	"ogg"
# define PNG	"png"
# define WAV	"wav"
# define WEBP	"webp"
# define WEBM	"webm"

# define DEFAULT_SUBTYPE "plain"

#endif