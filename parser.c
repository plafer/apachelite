#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "global_constants.h"
#include "structs.h"
#include "http_constants.h"
#include "util.h"

#include <stdio.h> /* TODO: REMOVE */

int parse_statusline(char **buf, int *size, struct request *conn_req);
int parse_httpmethod(char *method, int size, struct request *conn_req);
int parse_httpuri(char *uri, int size, struct request *conn_req);
int parse_httpversion(char *version, int size,
		      struct request *conn_req);

int parse_headers(char **buf, int *size, struct request *conn_req);

void skip_whitespace(char **buf, int *bufsize);
int is_endofword(char c);

/*
 * Parses a symbol in buf delimited by delimiter. Buffer pointer
 * will be moved to after the delimiter, skipping whitespaces.
 * Returns size of symbol.
 */
int parse_symbol(char **buf, int *const bufsize, char delimiter);

int parse_http(char *buf, int size, struct request *conn_req)
{
  if (parse_statusline(&buf, &size, conn_req))
    {
      return 1;
    }

  if (parse_headers(&buf, &size, conn_req))
    {
      return 1;
    }

  // Check if there is no payload with the request
  if (strlen(buf) == 0)
    {
      return 0;
    }

  

  return 0;
}


int parse_statusline(char **buf, int *const size,
		     struct request * conn_req)
{
  char *symbol = *buf;
  int symbol_size = parse_symbol(buf, size, ' ');
  
  if (*size <= 0 || parse_httpmethod(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  symbol = *buf;
  symbol_size = parse_symbol(buf, size, ' ');
  if (*size <= 0 || parse_httpuri(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  symbol = *buf;
  symbol_size = parse_symbol(buf, size, '\r');
  if (*size <= 0 || parse_httpversion(symbol, symbol_size, conn_req))
    {
      return 1;
    }

  return 0;
}

int parse_headers(char **buf, int *size, struct request *conn_req)
{
  char *hname, *hval;
  int hname_size, hval_size;
  while (**buf != '\r')
    {
      // header name
      hname = *buf;
      hname_size = parse_symbol(buf, size, ':');
      if (*size <= 0)
	{
	  return 1;
	}
      str_tolower(hname, hname_size);

      // header value
      hval = *buf;
      hval_size = parse_symbol(buf, size, '\r');
      if (*size <= 0)
	{
	  return 1;
	}

      if (!str_ismemberof(HTTP_REQUEST_HEADERS,
			  HTTP_REQUEST_HEADERS_SIZE,
			  hname,
			  hname_size))
	{
	  // If header not known, simply skip it.
	  continue;
	}

      // construct header
      struct header_node *header = construct_header_node(hname,
							 hname_size,
							 hval,
							 hval_size);
      request_add_header(conn_req, header);

    }

  // Remove CRLF
  (*buf) += 2;
  (*size) -= 2;
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
  // TODO: PARSE QUERY STRING IF GET REQUEST
  if (size <= 0)
    {
      return 1;
    }
  conn_req->uri = malloc(sizeof(char) * size + 1);
  strncpy(conn_req->uri, uri, size);
  conn_req->uri[size] = '\0';
  return 0;
}

int parse_httpversion(char *version, int size,
		      struct request *conn_req)
{
  if (size <= 0)
    {
      return 1;
    }
  
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

void skip_whitespace(char **buf, int *bufsize)
{
  while (*bufsize > 0)
    {
      if (**buf == ' ')
	{
	  (*buf)++;
	  (*bufsize)--;
	}
      else
	{
	  break;
	}
    }
}

int parse_symbol(char **buf, int *const bufsize, char delimiter)
{
  skip_whitespace(buf, bufsize);
  int wordsize = 0;
  while ( (*bufsize)-- > 0 )
    {
      if ( *(*buf)++ == delimiter )
	{
	  if (delimiter == '\r' && **buf == '\n')
	    {
	      (*buf)++;
	      (*bufsize)--;
	    }
	  break;
	}
      wordsize++;
    }

  skip_whitespace(buf, bufsize);

  return wordsize;
}
