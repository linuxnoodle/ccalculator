#include "common.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

int main(){
    char *input;
    Tokens t;
    Node *n;

    initialize_table();
    while (1){
        printf("> ");
        input = NULL;
        if (scanf("%m[^\n]%*c", &input) == 0){
            fprintf(stderr, "Error: string provided too long (somehow)\n");
            break;
        }

        t = lex(input);
        n = form_tree(&t);

        printf("%s = %f\n", input, evaluate_f(n));
        destroy_tree(n);
        destroy_tokens(&t);
        free(input);
    }
}
