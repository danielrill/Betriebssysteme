#include "hand_over_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


void Hand_List_Init(hand_list_t *L,  int threshold) {

    L ->head = NULL;
    L ->threshold = threshold;
    L ->size = 0;
    
    pthread_mutex_init(&L->insert_lock, NULL);
}

void Hand_List_Insert(hand_list_t *L, int key) {



    hand_lock_node_t *new_node = malloc(sizeof(hand_lock_node_t));
    assert(new_node != NULL);

    
    new_node -> key = key;
    
    pthread_mutex_init(&new_node->lock, NULL);
    
    
    
         // critical Section
        pthread_mutex_lock(&L->insert_lock);
        new_node->next = L->head;
        L->head = new_node;
    
    pthread_mutex_unlock(&L->insert_lock);
    
    L->size++;
    
}

int Hand_List_Lookup(hand_list_t *L, int key) {

    int counter = 0;
    hand_lock_node_t * tmp;
    
    int rv = -1;
   pthread_mutex_lock(&L->insert_lock);
   hand_lock_node_t *curr = L->head;
    while (curr) {
        if(counter % L->threshold == 0) {
            if (tmp == NULL) 
                continue;
                
            pthread_mutex_unlock(&tmp->lock);
            pthread_mutex_lock(&curr->lock);
            tmp = curr;
        }
         
        if (curr->key == key) {
        rv = 0;
      break;
    }
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->insert_lock);
  pthread_mutex_unlock(&tmp->lock);
  return rv; // now both success and failure

}

void Hand_List_Print(hand_list_t *L) {
     int counter = 0;
    hand_lock_node_t * tmp;

    hand_lock_node_t *curr = L->head;
    assert(L->head != NULL);
    
    pthread_mutex_lock(&curr -> lock);
    
     while (curr) {
        if(counter % L->threshold == 0) {
            if (tmp == NULL) 
                continue;
                
            pthread_mutex_unlock(&tmp->lock);
            pthread_mutex_lock(&curr->lock);
            tmp = curr;
        }
    printf(" key : %d\n", curr->key);
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->insert_lock);
  pthread_mutex_unlock(&tmp->lock);
    

}

void Hand_List_Free(hand_list_t *L) {
  pthread_mutex_lock(&L->insert_lock);
  hand_lock_node_t *curr = L->head;
  while (curr) {
    hand_lock_node_t *temp = curr;
    curr = curr->next;
    free(temp);
  }
  pthread_mutex_unlock(&L->insert_lock);
  free(L);
}


