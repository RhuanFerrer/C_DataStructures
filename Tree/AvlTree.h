#include <stdbool.h>
#include <stdio.h>

#include "../../DataStructures/HashTable/HashTable.h"
#include "../../DataStructures/DoublyLinkedList/DoublyLinkedList.h"

#ifndef __AVL_TREE_
#define __AVL_TREE_

typedef void* AvlTree;
typedef void* Data;
typedef void* Node;

typedef void (*FreeFunction)(void*);

/*
*
*   Retorna o no localizado a direita do no current
*
*/
Node avl_right(Node Current);

/*
*
*   Retorna o no localizado a esquerda do no current
*
*/
Node avl_left(Node Current);

/*
*
*   Retorna o no pai de current
*
*/
Node avl_father(Node Current);

Node avl_data(Node Currrent);

Node avl_root(AvlTree Tree);

AvlTree new_avl();

bool avl_is_empty(AvlTree Tree);

bool avl_insert(AvlTree Tree, void* Info, double (*compare_keys)(void*, void*));

void avl_print_dot_file(FILE* file, Node no);

bool avl_exists_key(AvlTree Tree, void* Info, double (*compare_keys)(void*, void*));

void* avl_remove(AvlTree Tree, void* data, double (*compare_keys)(void*, void*), double (*compare_data)(void*, void*));

List avl_get(AvlTree Tree, double x,double y,double w, double h) ;

void avl_symmetrical(Node node, void (*NodePrecessor)(Node));

void avl_destroy(AvlTree Tree, void (*free_func)(void*));

void memAvl();

HashTable avl_to_hash(AvlTree In, char* (*Key)(void*), double (*_compare)(void*, void*), char* (*_to_string)(void*), void (*free_func)(void*));

unsigned int avl_len(AvlTree in);

int max_level(Node no);

void avl_print_2D(Node No, int space, int (*print)(void*)) ;
void print_tree_recursion(Node No, int level, int* max_level);

#endif