#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "global_constants.h"

void free_map_list(struct map_node *head);

struct map_node *construct_map_node(char *name, int namelen,
					  char *value, int valuelen)
{
  struct map_node *node = malloc(sizeof(struct map_node));

  node->name = malloc(sizeof(char) * namelen + 1);
  strncpy(node->name, name, namelen);
  node->name[namelen] = '\0';

  node->value = malloc(sizeof(char) * valuelen + 1);
  strncpy(node->value, value, valuelen);
  node->value[valuelen] = '\0';

  node->next = NULL;
  
  return node;
}


void request_init(struct request *req)
{
  req->method = ENUM_DEFAULT_VALUE;
  req->uri = NULL;
  req->query_params = NULL;
  req->http_version = ENUM_DEFAULT_VALUE;
  req->headers = NULL;
  req->payload = NULL;
}

void request_add_header(struct request *req, struct map_node *header)
{
  header->next = req->headers;
  req->headers = header;
}

void request_add_queryparam(struct request *req, struct map_node *param)
{
  param->next = req->query_params;
  req->query_params = param;
}

void request_free(struct request *req)
{
  req->method = ENUM_DEFAULT_VALUE;
  free(req->uri);
  req->uri = NULL;
  free(req->payload);
  req->payload = NULL;
  req->http_version = ENUM_DEFAULT_VALUE;

  free_map_list(req->headers);
  req->headers = NULL;
  free_map_list(req->query_params);
  req->query_params = NULL;
}

void response_init(struct response *res)
{
  res->http_version = ENUM_DEFAULT_VALUE;
  res->status_code = NULL;
  res->reason_phrase = NULL;
  res->headers = NULL;
  res->payload = NULL;
}

void response_add_header(struct response *res, struct map_node *header)
{
  header->next = res->headers;
  res->headers = header;  
}

void response_free(struct response *res)
{
  res->http_version = ENUM_DEFAULT_VALUE;
  res->status_code = NULL;
  res->reason_phrase = NULL;
  
  free_map_list(res->headers);
  res->headers = NULL;
  free(res->payload);
  res->payload = NULL;
}

void free_map_list(struct map_node *head)
{
  struct map_node *next;
  while (head != NULL)
    {
      next = head->next;
      free(head->name);
      free(head->value);
      free(head);
      head = next;
    }
}


