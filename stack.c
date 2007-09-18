#include "stack.h"
#include "./string.h"
#include "message.h"
#include <stdlib.h>

stack *
stack_new (const unsigned alloc)
{
  MESSAGE_DEBUG("alloc:%u\n", alloc);
  stack *s = (stack *) calloc (1, sizeof (stack));
  if (!s)
    {
      MESSAGE_ERROR ("malloc failed\m");
      return NULL;
    }
  s->data = (void **) calloc (alloc, sizeof (void *));
  if (!s->data)
    {
      MESSAGE_ERROR ("malloc failed\n");
      return NULL;
    }
  s->size = alloc;
  s->current = 0;
  s->alloc = alloc;
  MESSAGE_DEBUG("return:%p\n", s);
  return s;
}

void
stack_delete (stack * s)
{
  MESSAGE_DEBUG("%p\n", s);
  free (s->data);
  free (s);
}

bool
stack_push (stack * s, void * val)
{
  MESSAGE_DEBUG("s:%p val:%d\n", s, val);
  if (s->current >= s->size)
    {
      s->data =
	(void **) realloc (s->data, sizeof (void *) * (s->size + s->alloc));
      if (s->data == NULL)
      {
    	  MESSAGE_DEBUG("return:false\n");
    	  return false;
      }
      s->size += s->alloc;
    }
  *(s->data + s->current++) = val;
  MESSAGE_DEBUG("return:true\n");
  return true;
}

void *
stack_pop (stack * s)
{
  MESSAGE_DEBUG("s:%p\n", s);
  if (s->current <= 0)
  {
    MESSAGE_ERROR("no data\n");
    return NULL;
  }
  MESSAGE_DEBUG("return:%d\n", *(s->data + s->current - 1));
  return *(s->data + --s->current);
}

void
stack_dump (stack * s)
{
  MESSAGE_DEBUG("s:%p\n", s);
  int i;
  for (i = 0; i < s->current; i++)
	  printf("%d:%p\n", i, *(s->data + i));
}
