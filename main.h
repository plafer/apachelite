#ifndef MAIN_H
#define MAIN_H

struct CLI_PARSER
{
  char *port;
};
  
struct CLI_PARSER cli_parser = {"-p"};


void print_usage(void);

#endif


