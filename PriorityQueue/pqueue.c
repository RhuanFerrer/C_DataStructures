#include "pqueue.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct do node da fila de prioridade
typedef struct node {
    double priority;
    void *data;
    struct node *next;
    struct node *prev;
} node;

// struct da fila de prioridade
typedef struct pqueue {
    node *head;
    node *tail;
    int size;

    char *(*_to_string)(void *);

    double (*graph_compare_vertex)(void *v1, void *v2);

} pqueue;

P_Queue new_pqueue(double graph_compare_vertex(void* v1, void* v2) ) {
    pqueue *pq = malloc(sizeof(pqueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    return pq;
}

void pqueue_print(P_Queue Pq) {
    pqueue *pq = Pq;
    node *aux = pq->head;
    printf("[");
    while (aux != NULL) {
        printf("(%s:%lf)", pq->_to_string(aux->data), aux->priority);
        aux = aux->next;
        if (aux != NULL)
            printf(", ");
    }
    printf("]\n");
}

bool pqueue_contains(P_Queue pq, void *data) {
    node *n = ((pqueue *)pq)->head;
     pqueue *Pq = pq;
    while (n != NULL) {
        if (Pq->graph_compare_vertex(n->data,data)) {
            return true;
        }
        n = n->next;
    }
    return false;
}

void pqueue_add(P_Queue pq, void *data, double priority) {
    pqueue *Pq = pq;
    //continue
    if (Pq->head == NULL) {
        node *newNode = malloc(sizeof(node));
        newNode->data = data;
        newNode->priority = priority;
        newNode->next = NULL;
        newNode->prev = NULL;
        Pq->head = newNode;
        Pq->tail = newNode;
        Pq->size++;
    } else {
        node *newNode = malloc(sizeof(node));
        newNode->data = data;
        newNode->priority = priority;
        newNode->next = NULL;
        newNode->prev = NULL;
        node *aux = Pq->head;
        while (aux != NULL) {
            if (aux->priority > priority) {
                if (aux->prev == NULL) {
                    newNode->next = aux;
                    aux->prev = newNode;
                    Pq->head = newNode;
                    Pq->size++;
                    break;
                } else {
                    newNode->next = aux;
                    newNode->prev = aux->prev;
                    aux->prev->next = newNode;
                    aux->prev = newNode;
                    Pq->size++;
                    break;
                }
            } else if (aux->next == NULL) {
                newNode->prev = aux;
                aux->next = newNode;
                Pq->tail = newNode;
                Pq->size++;
                break;
            } else {
                aux = aux->next;
            }
        }
    }
}

void pqueue_destroy(P_Queue pq, void (*destroy_data)(void *)) {
    pqueue *Pq = pq;
    node *aux = Pq->head;
    while (aux != NULL) {
        node *aux2 = aux;
        aux = aux->next;
        if (destroy_data != NULL) {
            destroy_data(aux2->data);
        }
        free(aux2);
    }
    free(Pq);
}

void *pqueue_pop(P_Queue pq) {
    pqueue *Pq = pq;
    if (Pq->head == NULL) {
        return NULL;
    } else {
        node *aux = Pq->head;
        if (!Pq->head->next) {
            Pq->head = NULL;
        } else {
            Pq->head = Pq->head->next;
            Pq->head->prev = NULL;
        }
        Pq->size--;
        return aux->data;
    }
}

int pqueue_size(P_Queue pq) {
    pqueue *Pq = pq;
    return Pq->size;
}