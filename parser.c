#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "global_constants.h"
#include "structs.h"
#include "http_constants.h"
#include "util.h"

#include <stdio.h>

int parse_statusline(char **buf, int *size, struct request *conn_req);
int parse_httpmethod(char *method, int size, struct request *conn_req);
int parse_httpuri(char *uri, int size, struct request *conn_req);

/*
 * Parses query string, starting AFTER the '?' delimiter.
 */
int parse_httpqueryparams(char *query, int size,
			  struct request *conn_req);
int parse_httpversion(char *version, int size,
		      struct request *conn_req);

int parse_headers(char **buf, int *size, struct request *conn_req);
int parse_payload(char *buf, int size, struct request *conn_req);

void skip_whitespace(char **buf, int *bufsize);
int is_endofword(char c);

/*
 * Finds the query string in the uri.
 * Returns a pointer to the delimiter '?' (or original pointer
 * if no query string) and sets params_size to the size of the
 * parameter string (starting at the delimiter).
 */
char *uri_find_queryparams(char *uri, int size, int *params_size);

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

  // Check if there is a payload with the request
  if (strlen(buf) == 0)
    {
      return 0;
    }

  if (parse_payload(buf, size, conn_req))
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
      struct map_node *header = construct_map_node(hname,
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
  if (conn_req->method == ENUM_DEFAULT_VALUE)
    {
      char *message = "Method should be parsed before uri. Also, "
	"This should return a 500-level response.\n";
      fprintf(stderr, message);
      exit(1);
    }
  
  if (size <= 0)
    {
      return 1;
    }

  int params_size = 0;
  char *params = uri_find_queryparams(uri, size, &params_size);

  if (conn_req->method == GET && params_size > 0)
    {
      size -= params_size;
      parse_httpqueryparams(params + 1, params_size - 1, conn_req);
    }
  
  conn_req->uri = malloc(sizeof(char) * size + 1);
  strncpy(conn_req->uri, uri, size);
  conn_req->uri[size] = '\0';

  return 0;
}

int parse_httpqueryparams(char *query, int size,
			  struct request *conn_req)
{
  const char PARAM_DEL = '&';
  const char KEYVALUE_DEL = '=';

  char *key = query, *value = NULL;
  int keylen = 0, valuelen = 0;

  int parsing_key = 1; // bool

  while (size-- > 0)
    {
      if (*query == PARAM_DEL || size == 0)
	{
	  parsing_key == 1 ? keylen++ : valuelen++;
	  struct map_node *param =
	    construct_map_node(key, keylen, value, valuelen);
	  request_add_queryparam(conn_req, param);

	  parsing_key = 1;
	  key = query + 1;
	  value = NULL;
	  keylen = valuelen = 0;
	}
      else if (*query == KEYVALUE_DEL)
	{
	  parsing_key = 0;
	  value = size > 0 ? query + 1: NULL;
	}
      else
	{
	  if (parsing_key)
	    {
	      keylen++;
	    }
	  else
	    {
	      valuelen++;
	    }
	}
      
      query++;
    }
  
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

int parse_payload(char *buf, int size, struct request *conn_req)
{
  if (size <= 1)
    {
      return 1;
    }
  if ((conn_req->payload = malloc (sizeof(char) * size)) == NULL)
    {
      // TODO: This is a bad strategy; this should be treated as a
      // 500 internal error, but it's going to be seen as 400s
      return 1;
    }
  strncpy(conn_req->payload, buf, size);
  conn_req->payload[size - 1] = '\0';

  return 0;
}


char *uri_find_queryparams(char *uri, int size, int *params_size)
{
  char *start = uri;
  *params_size = 0;
  while (size > 0)
    {
      if (*uri == '?')
	{
	  *params_size = size;
	  return uri;
	}
      uri++;
      size--;
    }
  
  return start;
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
