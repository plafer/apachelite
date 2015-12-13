#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "gbuf.h"
#include "structs.h"
#include "global_constants.h"

int send_httpresponse(struct response *conn_resp, int conn)
{
  if (conn_resp == NULL)
    {
      return 1;
    }

  struct gbuf response_gbuf;
  gbuf_init(&response_gbuf, 0);
  
  if ( conn_resp->http_version == ENUM_DEFAULT_VALUE ||
       conn_resp->status_code == NULL ||
       conn_resp->reason_phrase == NULL || conn_resp->headers == NULL ||
       conn_resp->payload == NULL)
    {
      char *min_resp = "HTTP/1.1 500 Internal Server Error\n"
	"Content-type: text/html\n"
	"Content-length: 30\n\n"
	"500 Internal Server Error.\n";
      gbuf_add(&response_gbuf, min_resp, strlen(min_resp));
      gbuf_nullterm(&response_gbuf);
    }
  else
    {
      // Not feeling like switch'ing the http version's enum
      gbuf_add(&response_gbuf, "HTTP/1.1 ", 9);
      
      gbuf_add(&response_gbuf, conn_resp->status_code,
	       strlen(conn_resp->status_code));
      gbuf_add(&response_gbuf, " ", 1);
      
      gbuf_add(&response_gbuf, conn_resp->reason_phrase,
	       strlen(conn_resp->reason_phrase));
      gbuf_add(&response_gbuf, "\r\n", 2);

      struct map_node *next_header = conn_resp->headers;
      while(next_header != NULL)
	{
	  gbuf_add(&response_gbuf, next_header->name,
		   strlen(next_header->name));
	  gbuf_add(&response_gbuf, ": ", 2);
	  gbuf_add(&response_gbuf, next_header->value,
		   strlen(next_header->value));
	  gbuf_add(&response_gbuf, "\r\n", 2);
	  next_header = next_header->next;
	}
      gbuf_add(&response_gbuf, "\r\n", 2);
      gbuf_add(&response_gbuf, conn_resp->payload,
	       strlen(conn_resp->payload));
      gbuf_nullterm(&response_gbuf);
    }

  send(conn, response_gbuf.data, response_gbuf.size, 0);
  
  gbuf_free(&response_gbuf);
  return 0;
}
