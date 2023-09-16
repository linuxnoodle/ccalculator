#include "stack.h"

stack *stack_new(size_t type_size){
    stack *s = malloc(sizeof(stack));
    s->type_size = type_size;
    s->capacity = 1;
    s->length = 0;
    s->index = 0;
    // data is void**, pointer to unknown pointer type
    s->data = malloc(type_size);
    return s;
}

void stack_push(stack *s, void *data){
    if (s->length == s->capacity){
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * s->type_size);
    }
    memcpy(s->data + s->length * s->type_size, data, s->type_size);
    ++s->length;
}

void *stack_pop(stack *s){
    if (s->length == 0){
        return NULL;
    }
    --s->length;
    return s->data + s->length * s->type_size;
}

void *stack_peek(stack *s){
    if (s->length == 0){
        return NULL;
    }
    return s->data + (s->length - 1) * s->type_size;
}

void free_stack(stack *s){
    free(s->data);
    free(s);
}
