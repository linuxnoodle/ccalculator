#include "parser.h"
#include "lexer.h"
#include "stack.h"
#include <stdlib.h>

// Break up an array of tokens into a parse tree
// Binary operators have a left and right child
// Function operators have an array of parameters

/* Example tree:
* "2 / 3 + 5sin(cos(2 + 2) + 2)"
*
* LEXED w/ PRECEDENCES:
* NUM = TOKEN_NUMBER
* OP = TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_POW
* FUNC = TOKEN_TEXT
* LP = TOKEN_LPAREN
* RP = TOKEN_RPAREN
*
* NUM OP NUM OP NUM FUNC LP FUNC LP NUM OP NUM RP OP NUM RP
* -1  2  -1  1  -1   5       5      -1   1  -1     1  -1  
*
* PARSED:
*         |+|
*         / \
*      |/|   |*|
*      / \   / \
*     2  3  5 |sin|
*               |
*               +
*              / \
*          |cos|  2
*            |
*           |+|
*           / \
*          2   2
*/

int *get_precedences(Tokens *tokens){
    int *precedences = malloc(sizeof(int) * tokens->length);
    int parentheses = 0;
    for (int i = 0; i < tokens->length; ++i){
        switch (tokens[i].tokens->type){
            case TOKEN_FALLBACK:
            case TOKEN_NUMBER:
                precedences[i] = -1;
                break;
            case TOKEN_PLUS:
            case TOKEN_MINUS:
                precedences[i] = 1 + parentheses * 10;
                break;
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
                precedences[i] = 2 + parentheses * 10;
                break;
            case TOKEN_POW:
                precedences[i] = 3 + parentheses * 10;
                break;
            case TOKEN_TEXT:
                precedences[i] = 4 + parentheses * 10;
                break;
            case TOKEN_LPAREN:
                ++parentheses;
                break;
            case TOKEN_RPAREN:
                --parentheses;
                break;
        }
    }
    if (parentheses != 0)
        fprintf(stderr, "Error: mismatched parentheses\n");
    return precedences;
}

Node *create_node(Token *t){
    Node *node = malloc(sizeof(Node));
    node->t = t;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *create_empty_node(){
    Token *t = malloc(sizeof(Token));
    char *s = "0";

    t->type = TOKEN_NUMBER;
    t->contents = s;
    t->length = 1;

    return create_node(t);
}

Node *recurse_collect(Tokens *tokens, int *precedence, size_t start, size_t end){
    if (start == end)
        return create_empty_node();

    int lowest_precedence = 1000;
    int lowest_precedence_index = -1;
    for (size_t i = start; i < end; ++i){
        if (precedence[i] > 0 && precedence[i] < lowest_precedence) {
            lowest_precedence = precedence[i];
            lowest_precedence_index = i;
        }
    }

    if (lowest_precedence_index == -1){
        // no operators found, must be a number
        // hacky but whatever
        if (tokens->tokens[start].type == TOKEN_LPAREN){
            return recurse_collect(tokens, precedence, start + 1, end);
        }
        return create_node(&tokens->tokens[start]);
    }

    Node *node = create_node(&tokens->tokens[lowest_precedence_index]);
    if (tokens->tokens[start].type == TOKEN_TEXT){
        node->left = create_empty_node();
        node->right = recurse_collect(tokens, precedence, start + 1, end);
        return node;
    } else {
        node->left = recurse_collect(tokens, precedence, start, lowest_precedence_index);
        node->right = recurse_collect(tokens, precedence, lowest_precedence_index + 1, end);
        return node;
    }
}

Node *parse(Tokens *tokens) {
    // iteratively create parse tree using stack
    // preserve order of operations
    stack *s = stack_new(sizeof(Node *));
    int *precedences = get_precedences(tokens);
    printf("\n");
    for (int i = 0; i < tokens->length; ++i){
        printf("%c: %d\n", tokens->tokens[i].contents[0], precedences[i]);
    }
    return recurse_collect(tokens, precedences, 0, tokens->length);
}

void print_tree(Node *root, int space){
    if (root == NULL)
        return;
    space += 4;
    print_tree(root->right, space);
    printf("\n");
    for (int i = 4; i < space; i++)
        printf(" ");
    printf("%s\n", root->t->contents);
    print_tree(root->left, space);
}

void destroy_tree(Node *root){
    if (root == NULL)
        return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}
