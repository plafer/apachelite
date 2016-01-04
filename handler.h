#ifndef HANDLER_H
#define HANDLER_H

/*
 * Handles connection from A to Z: reads from the
 * socket, parses, builds response, and sends.
 * Does not close the connection.
 */
void handle_connection(int conn);

#endif
