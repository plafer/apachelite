#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "global_constants.h"
#include "structs.h"
#include "http_constants.h"

#include <stdio.h> /* TODO: REMOVE */

int parse_statusline(char **buf, int *size, struct request *conn_req);
int parse_httpmethod(char *method, int size, struct request *conn_req);
int parse_httpuri(char *uri, int size, struct request *conn_req);
int parse_httpversion(char *version, int size,
		      struct request *conn_req);

int parse_headers(char **buf, int *size, struct request *conn_req);

int is_endofword(char c);

/*
 * Parses a symbol in buf delimited by delimiter. Buffer pointer
 * will be moved to after the delimiter.
 * Returns size of symbol.
 */
int parse_symbol(char **buf, int *const bufsize, char delimiter);

int parse_http(char *buf, int size, struct request *conn_req)
{
  if (parse_statusline(&buf, &size, conn_req))
    {
      return 1;
    }
  
  
  
  return 0;
}


int parse_statusline(char **buf, int *const size,
		     struct request * conn_req)
{
  char *symbol = *buf;
  int symbol_size = parse_symbol(buf, size, ' ');
  
  if (parse_httpmethod(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  symbol = *buf;
  symbol_size = parse_symbol(buf, size, ' ');
  if (parse_httpuri(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  symbol = *buf;
  symbol_size = parse_symbol(buf, size, '\n');
  if (parse_httpversion(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  return 0;
}

int parse_headers(char **buf, int *size, struct request *conn_req)
{
  
    
  return 0;
}

int parse_httpmethod(char *method, int size, struct request *conn_req)
{
  if (size <= 0)
    {
      return 1;
    }
  
  if (strncmp(method, "OPTIONS", size) == 0)
    {
      conn_req->method = OPTIONS;
    }
  else if (strncmp(method, "GET", size) == 0)
    {
      conn_req->method = GET;
    }
  else if (strncmp(method, "HEAD", size) == 0)
    {
      conn_req->method = HEAD;
    }
  else if (strncmp(method, "POST", size) == 0)
    {
      conn_req->method = POST;
    }
  else if (strncmp(method, "PUT", size) == 0)
    {
      conn_req->method = PUT;
    }
  else if (strncmp(method, "DELETE", size) == 0)
    {
      conn_req->method = DELETE;
    }
  else if (strncmp(method, "TRACE", size) == 0)
    {
      conn_req->method = TRACE;
    }
  else if (strncmp(method, "CONNECT", size) == 0)
    {
      conn_req->method = CONNECT;
    }
  else
    {
      conn_req->method = EXTENSION;
    }
  return 0;
}

int parse_httpuri(char *uri, int size, struct request *conn_req)
{
  conn_req->uri = malloc(sizeof(char) * size);
  strncpy(conn_req->uri, uri, size);
  return 0;
}

int parse_httpversion(char *version, int size,
		      struct request *conn_req)
{
  if (strncmp(version, "HTTP/1.1", size))
    {
      conn_req->http_version = HTTP11;
    }
  else if (strncmp(version, "HTTP/1.0", size))
    {
      conn_req->http_version = HTTP10;
    }
  else if (strncmp(version, "HTTP/0.9", size))
    {
      conn_req->http_version = HTTP09;
    }
  else
    {
      conn_req->http_version = UNKNOWN_VER;
    }
  return 0;
}

      
int is_endofword(char c)
{
  return c == ' ' || c == '\n';
}

int parse_symbol(char **buf, int *const bufsize, char delimiter)
{
  int wordsize = 0;
  while ( (*bufsize)-- > 0 )
    {
      if ( *(*buf)++ == delimiter )
	{
	  break;
	}
      wordsize++;
    }
  
  return wordsize;
}
