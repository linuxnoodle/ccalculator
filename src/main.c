#include "common.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "test.h"

int main(int argc, char **argv){
    if (argc == 2 && !strcmp(argv[1], "-t")){
        run_tests();
        return 0;
    }

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

        printf("%s = %g\n", input, evaluate_f(n));
        destroy_tree(n);
        destroy_tokens(&t);
        free(input);
    }
}
