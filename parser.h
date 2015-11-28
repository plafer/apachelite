#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

void parse_http(char *buf, int size, struct request *conn_req);


#endif
