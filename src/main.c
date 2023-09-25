#include "common.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "test.h"
#include "vars.h"

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
    char *r;

    initialize_table();
    initialize_vars();
    while (running){
        printf("$> ");
        fflush(stdout);

        input = getln(history, &history_size);
        printf("\n"); // raw deletes newline
        if (*input == '\0'){
            continue;
        }

        // print help message
        if (strcmp(input, "help") == 0){
            printf("Commands:\n");
            printf("  help - print this message\n");
            printf("  sin  - get sine of number\n");
            printf("  cos  - get cosine of number\n");
            printf("  tan  - get tangent of number\n");
            printf("  exp  - get e^x of number\n");
            printf("  ln   - get ln of number\n");
            printf("  set  - set calculator variable (radians)\n");
            continue;
        }
        
        is_valid = true;
        if (approximate){
            t = lex(input);
            n = parse(t);
            double d = evaluate_f(n);
            size_t len = snprintf(NULL, 0, "%f", d);
            r = malloc(sizeof(char) * (len + 1));
            sprintf(r, "%f", d);
        } else {
            char *last = malloc(sizeof(char) * (strlen(input) + 1));
            char *curr = input;
            while (strcmp(last, curr) != 0){
                free(last);
                last = curr;
                t = lex(curr);
                n = parse(t);
                curr = evaluate_exact(n);
                
                destroy_tree(n);
                destroy_tokens(&t);
            }
            r = curr;
        }

        if (is_valid){
            printf("%s = %s\n", input, r);
        }
        //printf("%s = %s\n", input, evaluate_exact(n));
        free(r);
        destroy_tree(n);
        destroy_tokens(&t);

        history = realloc(history, sizeof(char *) * (++history_size));
        history[history_size - 1] = input;
    }

    for (size_t i = 0; i < history_size; i++)
        free(history[i]);
    free(history);

    for (size_t i = 0; i < variable_count; i++){
        free(variables[i]->name);
        free(variables[i]->value);
        free(variables[i]);
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
