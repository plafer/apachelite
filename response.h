#ifndef RESPONSE_H
#define RESPONSE_H

#include "structs.h"

/*
 * Sends http response to connection, taking the information
 * from conn_resp.
 * In case of error, sends minimal 500 response and returns non-zero.
 */
int send_httpresponse(struct response *conn_resp, int conn);

#endif
