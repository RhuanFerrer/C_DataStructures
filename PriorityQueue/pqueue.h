#include <stdbool.h>

#ifndef PQUEUE_H

#define PQUEUE_H

typedef void* P_Queue;
typedef void* P_Queue_Node;

bool pqueue_contains(P_Queue pq, void* data);
P_Queue new_pqueue(double graph_compare_vertex(void* v1, void* v2) ) ;
void pqueue_add(P_Queue pq, void* data, double priority);
void* pqueue_pop(P_Queue pq);
void pqueue_print(P_Queue Pq);
void pqueue_destroy(P_Queue pq, void (*destroy_data)(void*));
int pqueue_size(P_Queue pq);
#endif