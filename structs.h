#ifndef STRUCTS_H
#define STRUCTS_H

#include "http_constants.h"


struct map_node
{
  char *name;
  char *value;
  struct map_node *next;
};

/*
 * Constructs a header node. keylen & valuelen are lengths that
 * would be returned from strlen; that is, without the null character.
 */
struct map_node *construct_map_node(char *key, int keylen,
					  char *value, int valuelen);

struct request
{
  HTTP_METHOD method;
  char *uri;
  struct map_node *query_params;
  HTTP_VER http_version;
  struct map_node *headers;
  char *payload;
};

void request_init(struct request *req);
void request_add_header(struct request *req, struct map_node *name);
void request_add_queryparam(struct request *req, struct map_node *name);
void request_free(struct request *req);


#endif
