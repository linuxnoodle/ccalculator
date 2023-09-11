#include "common.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

#include <signal.h>

void stop(int sig){ 
    printf("\nAre you sure you want to exit? (y/n) ");
    char c = getchar();
    switch (c){
        case 'n':
            printf("> ");
            break;
        case 'y':
            exit(0);
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Invalid input\n");
            break;
    }
}

int main(){
    // signal doohickeys
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = stop;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    char *input;
    size_t len;

    Tokens t;
    Node *n;

    initialize_table();
    while (1){
        printf("> ");
        getline(&input, &len, stdin);

        t = lex(input);
        n = form_tree(&t);

        printf("%s = %f\n", input, evaluate_f(n));
    }
    destroy_tree(n);
    destroy_tokens(&t);
    free(input);
}
