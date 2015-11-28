#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "init.h"
#include "structs.h"
#include "util.h"

void print_usage(void)
{
  printf("Usage: ./apachelite [-p PORT]\nDefaults to port 80.\n");
  exit(1);
}

int is_valid_port(int port)
{
  return port >= 0 && port <= 65535;
}

int main(int argc, char **argv)
{
  if (argc != 1 && argc != 3)
    {
      print_usage();
    }

  if (argc == 3 && (strcmp(argv[1], cli_parser.port) != 0 ||
		    !is_number(argv[2])))
    {
      print_usage();
    }

  int port = argc == 3 ? atoi(argv[2]) : 80;
  if (!is_valid_port(port))
    {
      print_usage();
    }
  
  create_server(port);

  return 0;
}

