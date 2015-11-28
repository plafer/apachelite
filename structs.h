#ifndef STRUCTS_H
#define STRUCTS_H

#include "http_constants.h"


struct header_node
{
  char *name;
  char *value;
  struct header_node *next;
};


struct request
{
  HTTP_METHOD method;
  char *uri;
  HTTP_VER http_version;
  struct header_node *headers;
  char *payload;
};

#endif
