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
        printf("size: %ld", t.length);
        for (size_t i = 0; i < t.length; ++i){
            printf("TOKEN: %s\n", t.tokens[i].contents);
        }
        n = form_tree(&t);
        print_tree(n, 0);

        if (!check_token_validity(&t)){
            destroy_tree(n);
            destroy_tokens(&t);
            free(input);
            continue;
        }

        printf("%s = %g\n", input, evaluate_f(n));
        destroy_tree(n);
        destroy_tokens(&t);
        free(input);
    }
}
