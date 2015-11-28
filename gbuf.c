#include <stdlib.h>
#include "stdio.h"

#include "gbuf.h"

#define GBUF_DEFAULT_SIZE 1024
#define GBUF_GROWTH_FACTOR 2

void gbuf_resize(struct gbuf *buf);

void gbuf_init(struct gbuf *buf, int desired_size)
{
  if (desired_size <= 0)
    {
      desired_size = GBUF_DEFAULT_SIZE;
    }
  if ((buf->data =  malloc(sizeof(char) * desired_size)) == NULL)
    {
      printf("No memory available for malloc... Not sure what"
	     " to do here.\n");
      exit(1);
    }
  buf->size = 0;
  buf->unused_segment = buf->data;
  buf->alloc_size = desired_size;
}  

void gbuf_add(struct gbuf *buf, char *toadd, int toadd_size)
{
  int remaining_size = buf->alloc_size - buf->size;
  while (toadd_size >= remaining_size)
    {
      gbuf_resize(buf);
      remaining_size = buf->alloc_size - buf->size;
    }
  
  buf->size += toadd_size;
  
  while (toadd_size-- > 0)
    {
      *buf->unused_segment++ = *toadd++;
    }
}

void gbuf_free(struct gbuf *buf)
{
  free(buf->data);
}


void gbuf_resize(struct gbuf *buf)
{
  // TODO: attempt realloc
  
  char *old_bufdata = buf->data;
  int new_size = buf->alloc_size * GBUF_GROWTH_FACTOR;
  if ((buf->data = malloc(sizeof(char) * new_size)) == NULL)
    {
      printf("No more memory available for malloc..."
	     " Not sure what to do here.\n");
      exit(1);
    }
  buf->alloc_size = new_size;

  int i;
  for (i = 0; i < buf->size; i++)
    {
      buf->data[i] = old_bufdata[i];
    }
  buf->unused_segment = buf->data + buf->size;
  
  free(old_bufdata);
  old_bufdata = NULL;
}
