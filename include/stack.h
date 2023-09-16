#ifndef STACK_H
#define STACK_H

#include "common.h"

typedef struct {
    void **data;
    size_t type_size, 
           capacity,
           length,
           index;
} stack;

stack *stack_new(size_t type_size);
void stack_push(stack *s, void *data);
void *stack_pop(stack *s);
void *stack_peek(stack *s);
size_t get_index();
void free_stack(stack *s);

#endif
