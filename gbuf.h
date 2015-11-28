#ifndef GBUF_H
#define GBUF_H

/*
 * Growable buffer
 */
struct gbuf
{
  char *data;
  int size;
  char *unused_segment;
  int alloc_size;
};

/*
 * Allocates the memory necessary for the growable 
 * buffer to function normally. If size less than 1 is
 * specified, 1024 is assumed.
 */
void gbuf_init(struct gbuf *buf, int desired_size);

/*
 * Add toadd to the gbuf buffer; will grow if needed.
 */
void gbuf_add(struct gbuf *buf, char *toadd, int toadd_size);

/*
 * Null terminates the buffer.
 */
void gbuf_nullterm(struct gbuf *buf);

/*
 * Frees memory associated with the buffer.
 */
void gbuf_free(struct gbuf *buf);

#endif
