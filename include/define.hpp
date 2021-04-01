#ifndef DEFINE_HPP
# define DEFINE_HPP

/*Server*/
# define WEBSERV "webserv"

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

#endif