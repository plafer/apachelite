#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "handler.h"
#include "parser.h"

#include "structs.h"
#include "global_constants.h"
#include "gbuf.h"

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
  
  parse_http(req_buffer.data, req_buffer.size, &conn_req);
  request_free(&conn_req);
  gbuf_free(&req_buffer);
  
  // TODO: handle request based on request method
  char resp[] = "HTTP/1.1 200 Unauthorized\n"
    "Content-type: text/html\n"
    "Content-length: 10\n\n"
    "Not Found.\n\n";
  send(conn, resp, strlen(resp) + 1, 0);
  
  close(conn);
}
