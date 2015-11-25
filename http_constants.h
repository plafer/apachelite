#ifndef HTTP_CONSTANTS_H
#define HTTP_CONSTANTS_H

typedef enum {OPTIONS,
	      GET,
	      HEAD,
	      POST,
	      PUT,
	      DELETE,
	      TRACE,
	      CONNECT} HTTP_METHOD;

typedef enum {HTTP09,
	      HTTP10,
	      HTTP11,
	      UNKNOWN_VER} HTTP_VER;


#endif
