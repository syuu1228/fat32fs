#ifndef LIST_H_
#define LIST_H_

typedef struct _list_node
{
  struct _list_node *prev, *next;
} list_node;

#define LIST_GET(node, name, type) (type *)((void *)node - offsetof(type, name))
#define LIST_FOR_EACH(node) for(; node; node = node->next)
#define LIST_FOR_EACH_REVERSE(node) for(; node; node = node->prev)
void list_insert_before (list_node * list, list_node * node);
void list_insert_after (list_node * list, list_node * node);
void list_delete (list_node * node);
void list_move_before (list_node * list, list_node * node);
void list_move_after (list_node * list, list_node * node);
list_node *list_head (list_node * node);
list_node *list_tail (list_node * node);
void list_node_copy (list_node * dest, list_node * src);
int list_count (list_node * node);
void list_dump (const list_node * const node);
#endif /*LIST_H_ */
