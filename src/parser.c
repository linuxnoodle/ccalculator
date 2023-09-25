#include "parser.h"
#include "hash.h"
#include "lexer.h"
#include "vars.h"
#include <stdio.h>
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

int *get_precedences(Tokens tokens){
    int *precedences = malloc(sizeof(int) * tokens.length);
    memset(precedences, -1, sizeof(int) * tokens.length);
    int parentheses = 0;
    for (int i = 0; i < tokens.length; ++i){
        switch (tokens.tokens[i].type){
            case TOKEN_FALLBACK:
            case TOKEN_NUMBER:
                precedences[i] = -1;
                break;
            case TOKEN_EQUALS:
                precedences[i] = 1;
                break;
            case TOKEN_PLUS:
            case TOKEN_MINUS:
                precedences[i] = 2 + parentheses * 10;
                break;
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
                precedences[i] = 3 + parentheses * 10;
                break;
            case TOKEN_POW:
                precedences[i] = 4 + parentheses * 10;
                break;
            case TOKEN_TEXT: {
                // check if function
                TEXT_ENUM t = hash_table[hash(tokens.tokens[i].contents)];
                if (t != 0 && !strcmp(tokens.tokens[i].contents, get_key(t)))
                    precedences[i] = 5 + parentheses * 10;
                else {
                    precedences[i] = -1; // is variable
                }
                break;
            }
            case TOKEN_LPAREN:
                ++parentheses;
                break;
            case TOKEN_RPAREN:
                --parentheses;
                break;
            default:
                break;
        }
    }
    if (parentheses != 0){
        fprintf(stderr, "Error: mismatched parentheses\n");
        fflush(stderr);
        is_valid = false;
    }

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

Node *recurse_collect(Tokens tokens, int *precedence, size_t start, size_t end){
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
        if (tokens.tokens[start].type == TOKEN_LPAREN){
            return recurse_collect(tokens, precedence, start + 1, end);
        }
        return create_node(&tokens.tokens[start]);
    }

    Node *node = create_node(&tokens.tokens[lowest_precedence_index]);
    if (tokens.tokens[lowest_precedence_index].type == TOKEN_TEXT){
        node->left = NULL;
        node->right = NULL;
        TEXT_ENUM name = hash_table[hash(tokens.tokens[start].contents)];
        if (name != 0 &&
            *tokens.tokens[start].contents == *get_key(name) && 
            !strcmp(tokens.tokens[start].contents + 1, get_key(name) + 1)){
            node->t->func.str = tokens.tokens[start].contents;
            node->t->func.text = name;
            // check if next token is parenthesis
            if (tokens.tokens[start + 1].type == TOKEN_LPAREN){
                // find matching parenthesis
                int parentheses = 1;
                size_t i = start + 2;
                while (parentheses > 0 && i < end){
                    if (tokens.tokens[i].type == TOKEN_LPAREN)
                        ++parentheses;
                    else if (tokens.tokens[i].type == TOKEN_RPAREN)
                        --parentheses;
                    ++i;
                }

                size_t groupings_index = 0;
                size_t *group_start = malloc(sizeof(size_t) * (i - start));
                group_start[0] = start + 2;
                for (size_t j = start + 2; j < i - 1; ++j){
                    if (tokens.tokens[j].type == TOKEN_COMMA){
                        ++groupings_index;
                        group_start[groupings_index] = j + 1;
                        group_start = realloc(group_start, sizeof(size_t) * (groupings_index + 1));
                    }
                }

                // for every grouping, recurse_collect and store in node->t->func.parameters
                node->t->func.param_count = groupings_index + 1;
                node->t->func.parameters = malloc(sizeof(Node*) * (groupings_index + 1));
                for (size_t j = 0; j < groupings_index + 1; ++j){
                    //node->t->func.parameters[j] = recurse_collect(tokens, precedence, group_starts[j], group_end[j]);
                    node->t->func.parameters[j] = recurse_collect(tokens, precedence, group_start[j], i - 1);
                }
            }
        } else {
            // just to prevent segfaults
            node->t->func.str = tokens.tokens[start].contents;
            node->t->func.text = FALLBACK; 
            if (check_if_variable(tokens.tokens[start].contents)){
                // act as if it's a number
                node->t->type = TOKEN_NUMBER;
                node->t->contents = get_contents(tokens.tokens[start].contents);
                node->t->length = tokens.tokens[start].length;
            } else {
                // add it as an undefined variable
                variable *v = malloc(sizeof(variable));
                v->name = tokens.tokens[start].contents;
                v->value = NULL;
                v->defined = false;
                add_variable(v);
                // still going to be FALLBACK LOL
            }
        }

        return node;
    } else {
        node->left = recurse_collect(tokens, precedence, start, lowest_precedence_index);
        node->right = recurse_collect(tokens, precedence, lowest_precedence_index + 1, end);
        return node;
    }
}

// generic wrapper function for recurse_collect
Node *parse(Tokens tokens) {
    int *precedences = get_precedences(tokens);
    return recurse_collect(tokens, precedences, 0, tokens.length);
}

void destroy_tree(Node *root){
    if (root == NULL)
        return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}
