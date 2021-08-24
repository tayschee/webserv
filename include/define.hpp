#ifndef DEFINE_HPP
# define DEFINE_HPP

/*Server*/
# define WEBSERV "webserv"
# define HTTP_VERSION "HTTP/1.1"
# define GATEWAY_INTERFACE "CGI/1.1"

/*line return*/
# define CRLF "\r\n"
# define SEPARATOR CRLF CRLF

# define STR_TO_REPLACE "XXX"
# define DEFAULT_ERROR_FILE_EXT ".html" /*to use a funtion simply*/

# define DEFAULT_ERROR_FILE	"<html>\n" \
							"<head><title>"STR_TO_REPLACE"</title></head>\n" \
							"<body>\n" \
							"<center><h1>"STR_TO_REPLACE"</h1></center>\n" \
							"<hr><center>"WEBSERV"</center>\n" \
							"</body>\n" \
							"</html>\n"

/*parser block*/
# define BLOCK_SERVER "server"
# define BLOCK_LOCATION "location"
# define BLOCK_ERROR "location_error"
# define BLOCK_CGI "cgi"
# define BLOCK_ROOT "root"

/*parser variable*/
# define ACCEPT "accept"
# define AUTH_BASIC_USER_FILE "auth_basic_user_file"
# define AUTH_BASIC "auth_basic"
# define BODY_SIZE "body_size_max"
# define AUTO_INDEX "autoindex"

/*define header field*/
# define ALLOW "Allow"
# define ACCEPT_CHARSET "Accept-Charset"
# define ACCEPT_LANGUAGE "Accept-Language"
# define AUTHORIZATION "Authorization"
# define CONTENT_LENGTH "Content-Length"
# define CONTENT_LOCATION "Content-Location"
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
# define TRANSFERT_ENCODING "Transfer-Encoding"
# define CONTENT_LANGUAGE "Content-Language"
# define WWW_AUTHENTICATE "WWW-Authenticate"

/*define header field for CGI*/
# define QUERY_STRING "QUERY_STRING"

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
# define STATUS_100 "Continue"
# define STATUS_101 "Switching Protocols"

# define STATUS_200 "OK"
# define STATUS_201 "Created"
# define STATUS_202 "Accepted"
# define STATUS_203 "Non-Authoritative Information"
# define STATUS_204 "No Content"
# define STATUS_205 "Reset Content"
# define STATUS_206 "Partial Content"

# define STATUS_300 "Multiple Choices"
# define STATUS_301 "Moved Permanently"
# define STATUS_302 "Found"
# define STATUS_303 "See Other"
# define STATUS_304 "Not Modified"
# define STATUS_305 "Use Proxy"
# define STATUS_307 "Temporary Redirect"

# define STATUS_400 "Bad Request"
# define STATUS_401 "Unauthorized"
# define STATUS_402 "Payment Required"
# define STATUS_403 "Forbidden"
# define STATUS_404 "Not Found"
# define STATUS_405 "Method Not Allowed"
# define STATUS_406 "Not Acceptable"
# define STATUS_407 "Proxy Authentification Required"
# define STATUS_408 "Request Timeout"
# define STATUS_409 "Conflict"
# define STATUS_410 "Gone"
# define STATUS_411 "Length Required"
# define STATUS_412 "Precondition Failde"
# define STATUS_413 "Payload Too Large"
# define STATUS_414 "URI Too Long"
# define STATUS_415 "Unsupported Media Type"
# define STATUS_417 "Expectation Failed"
# define STATUS_426 "Upgrade Required"

# define STATUS_500 "Internal Server Error"
# define STATUS_501 "Not Implemented"
# define STATUS_502 "Bad Gateway"
# define STATUS_503 "Service Unavailable"
# define STATUS_504 "Gateway Timeout"
# define STATUS_505 "HTTP Version Not Supported"
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

# define BMP	".bmp"
# define CSS 	".css"
# define HTML	".html"
# define GIF	".gif"
# define JPEG	".jpeg"
# define JPG	".jpg"
# define MP4	".mp4"
# define JS		".javascript"
# define OGG	".ogg"
# define PNG	".png"
# define WAV	".wav"
# define WEBP	".webp"
# define WEBM	".webm"

# define DEFAULT_SUBTYPE "plain"

#endif