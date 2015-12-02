#ifndef HTTP_CONSTANTS_H
#define HTTP_CONSTANTS_H

#define HTTP_REQUEST_HEADERS_SIZE 28
extern const char *const HTTP_REQUEST_HEADERS[];

typedef enum {OPTIONS,
	      GET,
	      HEAD,
	      POST,
	      PUT,
	      DELETE,
	      TRACE,
	      CONNECT,
	      EXTENSION} HTTP_METHOD;

typedef enum {HTTP09,
	      HTTP10,
	      HTTP11,
	      UNKNOWN_VER} HTTP_VER;


#endif
