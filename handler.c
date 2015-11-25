#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "handler.h"
#include "structs.h"

#define INC_BUFFER_SIZE 50

void handle_connection(int conn)
{
  char inc_buffer[INC_BUFFER_SIZE];
  int amount_received;
  while ((amount_received = recv(conn, &inc_buffer,
				 INC_BUFFER_SIZE, 0)) == INC_BUFFER_SIZE)
    {
      printf("%s", inc_buffer);
    }

  if (amount_received == 0)
	{
	  // connection is closed on other end
	  printf("Client closed the connection.\n");
	  close(conn);
	  return;
	}
      
  printf("%s", inc_buffer);
  char *send_msg = "HTTP/1.1 200 OK\n"
    "Content-type: text/html\n"
    "Content-length: 16\n\n"
    "it's all good!\n";
  send(conn, send_msg, strlen(send_msg) + 1, 0);

  close(conn);
}
