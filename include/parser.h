#ifndef PARSER
#define PARSER 

#include "common.h"
#include "lexer.h"
#include "hash.h"
#include "stack.h"

typedef struct Node Node;
struct Node {
    Token *t;
    Node *left, *right, *parent;
    function *f; // NULL by default
};

Node *parse(Tokens *tok);
void destroy_tree(Node *node);
void print_tree(Node* head, int space);

#endif /* end of include guard: PARSER */
