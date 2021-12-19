#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include "linked_list.h"
#include <pthread.h>


void List_Init(list_t *L) {
  L->head = NULL;
  pthread_mutex_init(&L->lock, NULL); 
}

void List_Insert(list_t *L, int key) {
  // synchronization not needed
  node_t *new = malloc(sizeof(node_t));
  if (new == NULL) {
    perror("malloc");
    return;
  }
  new->key = key;

  // just lock critical section
  pthread_mutex_lock(&L->lock);
  new->next = L->head;
  L->head = new;
  pthread_mutex_unlock(&L->lock);
}

int List_Lookup(list_t *L, int key) {
  int rv = -1;
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while (curr) {
    if (curr->key == key) {
      rv = 0;
      break;
    }
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->lock);
  return rv; // now both success and failure
}

void List_Print(list_t *L) {
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while (curr) {
    printf("%d\n", curr->key);
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->lock);
}

void List_Free(list_t *L) {
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while (curr) {
    node_t *temp = curr;
    curr = curr->next;
    free(temp);
  }
  pthread_mutex_unlock(&L->lock);
  free(L);
}
