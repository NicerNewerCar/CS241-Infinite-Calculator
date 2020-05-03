#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"

list *make_node(int num) {
    list *ptr;
    ptr = (list *)malloc(sizeof(list));
    assert(ptr != NULL);
    ptr->num = num;
    ptr->next = NULL;
    ptr->prev = NULL;
    return ptr;
}

void free_node(list *ptr) {
    free(ptr);
}

void show(list *start) {
	list *ptr;

	for(ptr = start; ptr->next != NULL; ptr=ptr->next) {
		fprintf(stderr,"[%d]->",ptr->num);
	}
  fprintf(stderr, "[%d]\n", ptr->num);
}
int list_length(list *start) {
  int x=0; list *ptr;
  if(start != NULL){
    for(ptr = start; ptr != NULL; ptr=ptr->next) {
      x++;
    }
  }
  return x;
}
