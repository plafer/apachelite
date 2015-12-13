#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "handler.h"
#include "parser.h"

#include "structs.h"
#include "global_constants.h"
#include "standard_responses.h"
#include "gbuf.h"
#include "util.h"
#include "response.h"

void handle_request(struct request *conn_req,
		    struct response *conn_resp);
void handle_get(struct request *conn_req,
		struct response *conn_resp);
void build_generalresponse(struct response *conn_resp, char *file_name,
			   char *status_code, char *reason_phrase);

void add_common_headers(struct response *conn_resp);

void handle_connection(int conn)
{
  struct request conn_req;
  request_init(&conn_req);

  char recv_buffer[RECV_BUFFER_SIZE];
  struct gbuf req_buffer;
  gbuf_init(&req_buffer, 0);

  int amount_received = RECV_BUFFER_SIZE;
  while (amount_received == RECV_BUFFER_SIZE)
    {
      amount_received = recv(conn, recv_buffer, RECV_BUFFER_SIZE, 0);
      if (amount_received == 0)
	{
	  // connection is closed on other end
	  printf("Client closed the connection.\n");
	  close(conn);
	  return;
	}
      gbuf_add(&req_buffer, recv_buffer, amount_received);
    }
  gbuf_nullterm(&req_buffer);

  struct response conn_resp;
  response_init(&conn_resp);
  conn_resp.http_version = HTTP11;

  if (parse_http(req_buffer.data, req_buffer.size, &conn_req))
    {
      build_generalresponse(&conn_resp, STANDARD_400,
			    "400", "Bad Request");
    }

  handle_request(&conn_req, &conn_resp);

  send_httpresponse(&conn_resp, conn);
  
  request_free(&conn_req);
  response_free(&conn_resp);
  gbuf_free(&req_buffer);

  close(conn);
}

void handle_request(struct request *conn_req,
		    struct response *conn_resp)
{
  switch(conn_req->method)
    {
    case GET:
      handle_get(conn_req, conn_resp);
      break;
    default:
      build_generalresponse(conn_resp, STANDARD_501,
			    "501", "Not Implemented");
    }
}

void handle_get(struct request *conn_req,
		struct response *conn_resp)
{
  if (conn_req->uri[0] != '/' || !ispath(conn_req->uri))
    {
      build_generalresponse(conn_resp, STANDARD_400,
			    "400", "Bad Request");
      return;
    }

  char *default_page = "";
  if(conn_req->uri[strlen(conn_req->uri) - 1] == '/')
    {
      default_page = DEFAULT_PAGE_NAME;
    }
  
  int req_path_len = strlen(SERVER_ROOT_DIRECTORY) +
    strlen(conn_req->uri) + strlen(default_page) + 3;
  char *req_path = malloc(req_path_len);
  memset(req_path, '\0', req_path_len);
  pathcat(SERVER_ROOT_DIRECTORY, conn_req->uri, req_path);
  if (ispath(default_page))
    {
      pathcat(req_path, default_page, req_path);
    }

  if (access(req_path, R_OK) != 0)
    {
      build_generalresponse(conn_resp, STANDARD_404,
			     "404", "Not Found");
    }
  else
    {
      build_generalresponse(conn_resp, req_path,
			     "200", "OK");
    }

  free(req_path);
}
void build_generalresponse(struct response *conn_resp, char *file_name,
			   char *status_code, char *reason_phrase)
{
  if (get_file_contents(file_name, &conn_resp->payload))
    {
      build_generalresponse(conn_resp, STANDARD_500,
			    "500", "Internal Server Error");
    }
  
  conn_resp->status_code = status_code;
  conn_resp->reason_phrase = reason_phrase;
  add_common_headers(conn_resp);
  
  // Content-Type
  char *contenttype_key = "Content-Type";
  char *contenttype_value = "text/html";
  struct map_node *contenttype_node =
    construct_map_node(contenttype_key, strlen(contenttype_key),
		       contenttype_value, strlen(contenttype_value));
  response_add_header(conn_resp, contenttype_node);

  char *contentlen_key = "Content-Length";
  char contentlen_value[20];
  // Yes yes, server won't handle large files
  sprintf(contentlen_value, "%d", (int) strlen(conn_resp->payload));
  struct map_node *contentlen_node =
    construct_map_node(contentlen_key, strlen(contentlen_key),
		       contentlen_value, strlen(contentlen_value));
  response_add_header(conn_resp, contentlen_node);

}

void add_common_headers(struct response *conn_resp)
{
  // Date
  char *date_key = "Date";
  char date_value[1000];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(date_value, sizeof date_value, "%a, %d %b %Y %H:%M:%S %Z",
	   &tm);
  struct map_node *date_node =
    construct_map_node(date_key, strlen(date_key),
		       date_value, strlen(date_value));
  response_add_header(conn_resp, date_node);

  // Connection
  char *connection_key = "Connection";
  char *connection_value = "close";
  struct map_node *connection_node =
    construct_map_node(connection_key, strlen(connection_key),
		       connection_value, strlen(connection_value));
  response_add_header(conn_resp, connection_node);

  // Server
  char *server_key = "Server";
  char *server_value = "Apachelite/0.1";
  struct map_node *server_node =
    construct_map_node(server_key, strlen(server_key),
		       server_value, strlen(server_value));
  response_add_header(conn_resp, server_node);
}
