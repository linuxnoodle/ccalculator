#include "parser.h"

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
        return create_empty_node();
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
    if (tok->tokens[start].type == TOKEN_TEXT){ // fill with all parameters
        node->left = create_empty_node();
        node->right = recurse_collect(tok,
                                     precedence,
                                     lowest_precedence_index + 1,
                                     end);
        
        return node;
    } else {
        node->left = recurse_collect(tok,
                                     precedence,
                                     start,
                                     lowest_precedence_index);
    }
    node->right = recurse_collect(tok,
                                  precedence,
                                  lowest_precedence_index + 1,
                                  end);
    return node;
}

Node *form_tree(Tokens *tok){
    char *precedence = malloc(sizeof(char) * tok->length);
    memset(precedence, 0, sizeof(char) * tok->length);
    // generate precedence for each operator 
    int paren_depth = 0;
    for (int i = 0; i < tok->length; ++i){
        // check if text is a function name through hashtable 
        // if so, set precedence to 1
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
                    precedence[i] = 10 * paren_depth; 
                break;
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
                precedence[i] = 2;
                if (paren_depth != 0)
                    precedence[i] = 10 * paren_depth; 
                break;
            case TOKEN_POW:
                precedence[i] = 3;
                if (paren_depth != 0)
                    precedence[i] = 10 * paren_depth;
                break;
            case TOKEN_TEXT: {
                TEXT_ENUM name = hash_table[hash(tok->tokens[i].contents)];
                if (name != 0
                && *tok->tokens[i].contents == *get_key(name)
                && !strcmp(tok->tokens[i].contents + 1, get_key(name) + 1)){ 
                    precedence[i] = 5;
                    tok->tokens[i].func.text = name;
                    continue;
                }
                break;
            }
            default:
                precedence[i] = -1;
                break;
        }
    }

    return recurse_collect(tok,
                           precedence,
                           0,
                           tok->length);
}

void print_tree(Node* head, int space){
    if (head == NULL)
        return;

    space += 4;
    print_tree(head->right, space);

    printf("\n");
    for (int i = 4; i < space; ++i)
        printf(" ");
    if (head->t != NULL)
        printf("%s\n", head->t->contents);

    print_tree(head->left, space);
}
