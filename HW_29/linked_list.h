#include <pthread.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node_t {
  struct node_t *next;
  int key;
} node_t;

typedef struct list_t {
  node_t *head;
  pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L);

void List_Insert(list_t *L, int key);

int List_Lookup(list_t *L, int key);

void List_Print(list_t *L);

void List_Free(list_t *L);


#endif
