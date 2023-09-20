#include "common.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "test.h"

bool running = true;
char* getln(char **history, size_t *history_size){
    char *line = NULL;
    char c = 0;
    size_t size = 0;
    while (read(STDIN_FILENO, &c, 1) && c != '\n'){
        switch (c){
            // ctrl + c
            case 3:
                printf("exiting...\n");
                free(line);
                exit(0);
            // backspace
            case 127:
                if (size > 1){
                    line = realloc(line, --size);
                    printf("\b \b");
                    fflush(stdout);
                } else if (size == 1){
                    free(line);
                    line = NULL;
                    size = 0;
                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
        }
        if (!iscntrl(c)){
            line = realloc(line, ++size);
            line[size - 1] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    line = realloc(line, ++size);
    line[size - 1] = '\0';
    return line;
}

int main(int argc, char **argv){
    if (argc == 2 && !strcmp(argv[1], "-t")){
        run_tests();
        return 0;
    }
    
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON | ECHOE);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    char **history = NULL;
    size_t history_size = 0;

    char *input;
    Tokens t;
    Node *n;

    initialize_table();
    while (running){
        printf("> ");
        fflush(stdout);

        input = getln(history, &history_size);
        if (*input == '\0'){
            printf("\n");
            continue;
        }
        t = lex(input);
        printf("\n");
        /*for (size_t i = 0; i < t.length; i++)
            printf("%s: ID%d\n", t.tokens[i].contents, t.tokens[i].type);*/
        n = parse(t);
        //print_tree(n, 0);

        /*if (!check_token_validity(&t)){
            destroy_tree(n);
            destroy_tokens(&t);
            continue;
        }*/ // skipping for now LOL

        printf("\n%s = %g\n", input, evaluate_f(n));
        //printf("%s = %s\n", input, evaluate_exact(n));
        destroy_tree(n);
        destroy_tokens(&t);

        history = realloc(history, sizeof(char *) * (++history_size));
        history[history_size - 1] = input;
    }

    for (size_t i = 0; i < history_size; i++)
        free(history[i]);
    free(history);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
