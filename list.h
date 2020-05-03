#ifndef LIST_H
#define LIST_H

typedef struct list_s {
    int num;
    struct list_s *next;
    struct list_s *prev;
} list;

list *make_node(int);
void free_node(list *);
void show(list *);
int list_length(list *);

#endif
