#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    void* data;
    struct node *down;
}node;

typedef struct stack{
    int size;
    struct node *top;
}stack;

Stack new_stack(){
    stack *s = malloc(sizeof(stack));
    s->size = 0;
    s->top = NULL;
    return s;
}

void stack_add(Stack S, void* data){
   stack *s = (stack*)S;
    node *n = malloc(sizeof(node));
    n->data = data;
    n->down = s->top;
    s->top = n;
    s->size++;
}

void* stack_pop(Stack S){
    stack *s = (stack*)S;
    if(s->size == 0){
        return NULL;
    }
    node *n = s->top;
    s->top = n->down;
    s->size--;
    return n->data;
}

int stack_size(Stack S){
    stack *s = (stack*)S;
    return s->size;   
}