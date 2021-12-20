#ifndef STACK_H
#define STACK_H

typedef void* Stack;


Stack new_stack();
void stack_add(Stack S, void* data);
void* stack_pop(Stack S);
int stack_size(Stack S);
#endif /* STACK_H */