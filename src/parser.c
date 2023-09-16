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
        switch (tokens->tokens->type){
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

Node *parse(Tokens *tokens){
    // iteratively create parse tree using stack
    // preserve order of operations
    stack *s = stack_new(sizeof(Node *));
    int *precedences = get_precedences(tokens);

    Node *root, *current;
    root = current = malloc(sizeof(Node));
    
    // push tokens to stack according to precedence
    while (s->length > 0 || current != NULL){
        if (current != NULL){
            stack_push(s, &current);
            current = NULL;
        } else {
            current = *(Node **)stack_pop(s);
            if (current->t->type == TOKEN_LPAREN){
                free(current);
                current = NULL;
            } else if (current->t->type == TOKEN_RPAREN){
                free(current);
                current = *(Node **)stack_pop(s);
                if (current->t->type != TOKEN_LPAREN){
                    fprintf(stderr, "Error: mismatched parentheses\n");
                    return NULL;
                }
                free(current);
                current = NULL;
            } else {
                if (s->length == 0)
                    break;
                Node *parent = *(Node **)stack_peek(s);
                if (precedences[parent->t->index] < precedences[current->t->index]){
                    // parent has lower precedence, so it's a child of current
                    current->left = parent;
                    *(Node **)stack_pop(s);
                    stack_push(s, &current);
                    current = NULL;
                } else {
                    // parent has higher precedence, so current is a child of parent
                    parent->right = current;
                    current = parent;
                }
            }
        }
    }
}

void print_tree(Node *root, int space){
    if (root == NULL)
        return;
    space += 10;
    print_tree(root->right, space);
    printf("\n");
    for (int i = 10; i < space; ++i)
        printf(" ");
    printf("%d\n", root->t->type);
    print_tree(root->left, space);
}

void destroy_tree(Node *root){
    if (root == NULL)
        return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}
