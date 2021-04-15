#ifndef DEFINE_HPP
# define DEFINE_HPP

/*Server*/
# define WEBSERV "webserv"

/*line return*/
# define CRLF "\r\n"

/*delete file message*/
# define DELETE_FILE	"<html>"\
						"	<body>"\
    					"		<h1>File deleted.</h1>"\
						"	</body>"\
						"</html>"

# define ERROR_FILE_NAME ".html" /*to use a funtion simply*/

# define ERROR_FILE		"<html>\n"\
						"<head><title>xxx</title></head>\n"\
						"<body>\n"\
						"<center><h1>xxx</h1></center>\n"\
						"<hr><center>webserv</center>\n"\
						"</body>\n"\
						"</html>"

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
# define TRANSFERT_ENCODING "Tranfert-Encoding"
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
# define STATUS_405 "Method Not Allowed"
# define STATUS_406 "Not Acceptable"
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

/*define encoding extension*/
# define GZIP		"gz"
# define COMPRESS	"Z"
# define DEFLATE	""
#endif