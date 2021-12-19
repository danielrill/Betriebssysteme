#include <pthread.h>

#ifndef HAND_OVER_LIST_H
#define HAND_OVER_LIST_H

typedef struct hand_lock_node_t {
  struct hand_lock_node_t *next;
  pthread_mutex_t lock;
  int key;
} hand_lock_node_t;


typedef struct hand_list_t {
  hand_lock_node_t *head;
  pthread_mutex_t insert_lock;
  int threshold;
  int size;
} hand_list_t;

void Hand_List_Init(hand_list_t *L, int threshold);

void Hand_List_Insert(hand_list_t *L, int key);

int Hand_List_Lookup(hand_list_t *L, int key);

void Hand_List_Print(hand_list_t *L);

void Hand_List_Free(hand_list_t *L);


#endif
