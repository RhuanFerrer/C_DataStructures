#include "DoublyLinkedList.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    void* data;
    struct node* list_next;
    struct node* list_prev;

} node;

typedef struct list {
    node* list_head;
    node* list_tail;
    size_t len;

    double (*_compare)(void*, void*);
    char* (*_to_string)(void*);
    void (*free_func)(void*);

} list;

void verMem() {
    printf("Tamanho da Celula = %ld\nTamanho da Lista = %ld\n", sizeof(node), sizeof(list));
}

Node list_next(Node _node) {
    return _node ? ((node*)_node)->list_next : NULL;
}

void* list_search(List _list, void* data) {
    Node runner;
    list* List = (list*)_list;

    for (runner = list_head(_list); runner; runner = list_next(runner)) {
        if (List->_compare(list_data(runner), data))
            return runner;
    }

    return NULL;
}

void* list_to_array(List _list) {
    list* List = (list*)_list;
    void** array = malloc(sizeof(void*) * List->len);
    Node runner;
    size_t i = 0;

    for (runner = list_head(_list); runner; runner = list_next(runner)) {
        array[i++] = list_data(runner);
    }

    return array;
}

Node list_prev(Node _node) {
    return _node ? ((node*)_node)->list_prev : NULL;
}

void* list_data(Node _node) {
    return _node ? ((node*)_node)->data : NULL;
}

List new_list(double (*_compare)(void*, void*), char* (*_to_string)(void*), void (*free_func)(void*)) {
    list* new_list = calloc(1, sizeof(list));
    new_list->list_head = new_list->list_tail = NULL;
    new_list->len = 0;
    new_list->_compare = _compare;
    new_list->_to_string = _to_string;
    new_list->free_func = free_func;
    return new_list;
}

void list_set_data(Node no, void* data) {
    node* n = no;
    n->data = data;
}

Node list_head(List _list) {
    return _list ? ((list*)_list)->list_head : NULL;
}

Node list_tail(List _list) {
    return _list ? ((list*)_list)->list_tail : NULL;
}

size_t list_len(List _list) {
    return _list ? ((list*)_list)->len : 0;
}

Node list_index(List _list, unsigned int _index) {
    if (!_list || _index == -1) return NULL;

    Node runner;
    unsigned int cont;

    for (runner = list_head(_list), cont = 0; list_next(runner) && cont < _index; runner = list_next(runner), cont++)
        ;

    return runner;
}

void list_insert(Node _list, void* _data) {
    if (!_list || !_data) return;

    list* List = _list;

    node* new_node = calloc(1, sizeof(node));

    if (!List->len) {
        List->list_head = new_node;
        new_node->list_prev = NULL;
    } else {
        List->list_tail->list_next = new_node;
        new_node->list_prev = List->list_tail;
    }

    new_node->data = _data;
    new_node->list_next = NULL;

    List->list_tail = new_node;
    List->len++;
}

void* list_remove(List _list, unsigned int _index) {
    if (!_list || !list_len(_list)) return NULL;

    list* List = _list;
    node* aux;

    if (List->len == 1) {
        aux = List->list_head;
        List->list_head = NULL;
        List->list_tail = NULL;
    } else if (!_index) {
        aux = List->list_head;
        List->list_head = List->list_head->list_next;
        List->list_head->list_prev = NULL;
    } else if (_index >= List->len - 1) {
        aux = List->list_tail;
        List->list_tail = List->list_tail->list_prev;
        List->list_tail->list_next = NULL;
    } else {
        aux = list_index(List, _index);
        aux->list_prev->list_next = aux->list_next;
        aux->list_next->list_prev = aux->list_prev;
    }

    void* data = aux->data;

    free(aux);

    List->len--;

    if (!List->len)
        List->list_head = NULL;

    return data;
}

void list_remove_and_destroy(List _list, unsigned int _index) {
    if (!_list || !list_len(_list)) return;

    list* List = _list;
    node* aux;

    if (List->len == 1) {
        aux = List->list_head;
        List->list_head = NULL;
        List->list_tail = NULL;
    } else if (!_index) {
        aux = List->list_head;
        List->list_head = List->list_head->list_next;
        List->list_head->list_prev = NULL;
    } else if (_index >= List->len - 1) {
        aux = List->list_tail;
        List->list_tail = List->list_tail->list_prev;
        List->list_tail->list_next = NULL;
    } else {
        aux = list_index(List, _index);
        aux->list_prev->list_next = aux->list_next;
        aux->list_next->list_prev = aux->list_prev;
    }

    List->len--;

    if (!List->len)
        List->list_head = NULL;

    List->free_func(aux->data);
    free(aux);
}

int list_index_of(List _list, void* data) {
    Node runner;
    list* List = (list*)_list;

    int index = 0;

    for (runner = list_head(_list); runner; runner = list_next(runner)) {
        if (List->_compare(list_data(runner), data)) {
            return index;
        }
        index++;
    }
    return -1;
}

void list_print(List _list) {
    Node runner;
    list* List = (list*)_list;
    if (!_list)
        return;
    printf("[");
    for (runner = list_head(_list); runner; runner = list_next(runner)) {
        char* string = List->_to_string(list_data(runner));
        printf("%s", string);
        free(string);
        if (list_next(runner))
            printf(", ");
    }
    printf("]\n");
}

void list_sort(List _list) {
    if (!_list || !list_len(_list)) return;

    list* List = _list;
    node* aux;

    for (node* runner = List->list_head; runner; runner = runner->list_next) {
        for (node* runner2 = runner->list_next; runner2; runner2 = runner2->list_next) {
            if (List->_compare(runner->data, runner2->data) > 0) {
                aux = runner->data;
                runner->data = runner2->data;
                runner2->data = aux;
            }
        }
    }
}

void list_destroy(List _list) {
    if (!_list) return;
    list* List = _list;

    do {
        List->free_func(list_remove(_list, 0));
    } while (list_head(_list));

    free(_list);
}

void list_new_head(List _list, void* data) {
    list* l = _list;
    node* new = calloc(1, sizeof(node));
    new->data = data;
    new->list_next = new->list_prev = NULL;
    if (l->len == 0) {
        l->list_head = l->list_tail = new;
    } else {
        new->list_next = l->list_head;
        l->list_head->list_prev = new;
        l->list_head = new;
    }
    l->len++;
}