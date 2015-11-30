#include <stdlib.h>

#include "structs.h"

void request_init(struct request *req)
{
  req->uri = NULL;
  req->headers = NULL;
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
