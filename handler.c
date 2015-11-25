#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "handler.h"
#include "structs.h"
#include "global_constants.h"

void handle_connection(int conn)
{
  struct request conn_req;
  
  char inc_buffer[INC_BUFFER_SIZE];
  int amount_received = -1;
  int receive_more = 1;
  while (amount_received == INC_BUFFER_SIZE)
    {
      amount_received = recv(conn, inc_buffer, INC_BUFFER_SIZE, 0);

      if (amount_received == 0)
	{
	  // connection is closed on other end
	  printf("Client closed the connection.\n");
	  close(conn);
	  return;
	}
      
      parse(inc_buffer, amount_received, &conn_req);
    }

  // TODO: handle request based on request method

  close(conn);
}
