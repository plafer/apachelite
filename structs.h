#ifndef STRUCTS_H
#define STRUCTS_H

#include "http_constants.h"


struct header_node
{
  char *name;
  char *value;
  struct header_node *next;
};

/*
 * Constructs a header node. namelen & valuelen are lengths that
 * would be returned from strlen; that is, without the null character.
 */
struct header_node *construct_header_node(char *name, int namelen,
					  char *value, int valuelen);

struct request
{
  HTTP_METHOD method;
  char *uri;
  HTTP_VER http_version;
  struct header_node *headers;
  char *payload;
};

void request_init(struct request *req);
void request_add_header(struct request *req, struct header_node *name);
void request_free(struct request *req);


#endif
