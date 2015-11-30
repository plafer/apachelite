#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

/*
 * Returns 0 for success, which conn_req will be properly filled;
 * non-zero if buf doesn't contain proper HTTP (401 bad request).
 * Memory: This function allocates memory inside struct request,
 * that is for uri and headers.
 */
int parse_http(char *buf, int size, struct request *conn_req);


#endif
