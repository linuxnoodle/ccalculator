#ifndef PARSER
#define PARSER 

#include "common.h"
#include "lexer.h"

struct Node {
    char *contents;
};
typedef struct Node Node;

struct Tree {}
    Node *node;
    struct Tree *left;
    struct Tree *right;
};

#endif /* end of include guard: PARSER */
