#ifndef STACK_H_
#define STACK_H_
#include "./type.h"
typedef struct
{
  int size, current;
  unsigned alloc;
  void **data;
} stack;

#define STACK_FOR_EACH(s) for(; s->current > 0;)

stack *stack_new (const unsigned alloc);
void stack_delete (stack * s);
bool stack_push (stack * s, void *val);
void *stack_pop (stack * s);
void stack_dump (stack * s);
#endif /*STACK_H_ */
