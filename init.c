#include <netinet/in.h> /* sockaddr_in */
#include <arpa/inet.h> /* htons */
#include <string.h> /* memset */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close */
#include "init.h"
#include "handler.h"

static const int BACKLOG = 10;

int create_server(int port)
{
  int listenersock;
  struct sockaddr_in inc_addr;
  inc_addr.sin_family = AF_INET;
  inc_addr.sin_port = htons(port);
  inc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(&(inc_addr.sin_zero), '\0', 8);
  
  if ((listenersock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("socket");
      exit(1);
    }

  if (bind(listenersock, (struct sockaddr *) &inc_addr,
	   sizeof(struct sockaddr)) == -1)
    {
      perror("bind");
      exit(1);
    }

  // lose the "Address in use" error message
  int yes = 1;
  if (setsockopt(listenersock, SOL_SOCKET, SO_REUSEADDR, &yes,
		 sizeof(int)) == -1)
    {
      perror("setsockopt");
      exit(1);
    }
  
  if (listen(listenersock, BACKLOG) == -1)
    {
      perror("listen");
      exit(1);
    }
  printf("Rock and Rollin'\n");

  int new_connection;
  struct sockaddr_in ext_addr;
  unsigned int ext_addr_size = sizeof(struct sockaddr_in);

  // Consider forking all new connections
  while ((new_connection = accept(listenersock,
			       (struct sockaddr *) &ext_addr,
			       &ext_addr_size)) != -1)
    {
      handle_connection(new_connection);
    }
  if (new_connection == -1)
    {
        perror("accept");
	exit(1);
    }
  

  return 1;
}
