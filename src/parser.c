#include "parser.h"

Node *create_node(Token *t){
    Node *node = malloc(sizeof(Node));
    node->t = t;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void destroy_tree(Node *node){
    if (node == NULL){
        return;
    }
    destroy_tree(node->left);
    destroy_tree(node->right);
    free(node);
}

Node *recurse_collect(Tokens *tok, char *precedence, size_t start, size_t end){
    if (start == end){
        return NULL; // will probably be an issue eventually
    }
    // find the operator with the lowest precedence
    int lowest_precedence = 100;
    int lowest_precedence_index = -1;
    for (size_t i = end - 1; i-- > start;){
        if (precedence[i] > 0 && precedence[i] < lowest_precedence){
            lowest_precedence = precedence[i];
            lowest_precedence_index = i;
        }
    }
    if (lowest_precedence_index == -1){
        // no operator found, must be a number
        //printf("Number found: %f\n", atof(tok->tokens[start].contents));
        
        // if parenthesis, go to the next token
        // hacky but whatever
        if (tok->tokens[start].type == TOKEN_LPAREN){
            return recurse_collect(tok, precedence, start + 1, end);
        }
        return create_node(&tok->tokens[start]);
    } /*else {
        printf("Operator found: %s\n", tok->tokens[lowest_precedence_index].contents);
    }*/
    Node *node = create_node(&tok->tokens[lowest_precedence_index]);
    node->left = recurse_collect(tok, precedence, start, lowest_precedence_index);
    node->right = recurse_collect(tok, precedence, lowest_precedence_index + 1, end);
    return node;
}

Node *form_tree(Tokens *tok){
    char *precedence = malloc(sizeof(char) * tok->length);
    memset(precedence, 0, sizeof(char) * tok->length);

    // generate precedence for each operator 
    int paren_depth = 0;
    for (int i = 0; i < tok->length; ++i){
        switch (tok->tokens[i].type){
            case TOKEN_LPAREN:
                ++paren_depth;
                break;
            case TOKEN_RPAREN:
                --paren_depth;
                break;
            case TOKEN_PLUS:
            case TOKEN_MINUS:
                precedence[i] = 1;
                // technically this is wrong but it works for now
                if (paren_depth != 0)
                    precedence[i] = 10; 
                break;
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
                precedence[i] = 2;
                if (paren_depth != 0)
                    precedence[i] = 10; 
                break;
            default:
                precedence[i] = -1;
        }
    }

    return recurse_collect(tok, precedence, 0, tok->length);
}
