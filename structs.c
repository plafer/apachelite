#include <stdlib.h>
#include <string.h>
#include "structs.h"

struct header_node *construct_header_node(char *name, int namelen,
					  char *value, int valuelen)
{
  struct header_node *node = malloc(sizeof(struct header_node));

  node->name = malloc(sizeof(char) * namelen + 1);
  strncpy(node->name, name, namelen);
  node->name[namelen] = '\0';

  node->value = malloc(sizeof(char) * valuelen + 1);
  strncpy(node->value, value, valuelen);
  node->value[valuelen] = '\0';

  return node;
}


void request_init(struct request *req)
{
  req->uri = NULL;
  req->headers = NULL;
}

void request_add_header(struct request *req, struct header_node *header)
{
  header->next = req->headers;
  req->headers = header;
}

void request_free(struct request *req)
{
  free(req->uri);
  struct header_node *next_header;
  while (req->headers != NULL)
    {
      next_header = req->headers->next;
      free(req->headers->name);
      free(req->headers->value);
      free(req->headers);
      req->headers = next_header;
    }
}
