#ifndef PARSER
#define PARSER 

#include "common.h"
#include "lexer.h"
#include "hash.h"

typedef struct Node Node;
struct Node {
    Token *t;
    Node *left, *right;
};

Node *form_tree(Tokens *tok);
void destroy_tree(Node *node);
void print_tree(Node* head, int space);

#endif /* end of include guard: PARSER */
