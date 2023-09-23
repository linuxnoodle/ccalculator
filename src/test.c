#include "test.h"

#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "hash.h"

bool file_exists(const char *path) {
    struct stat buffer;
    return !stat(path, &buffer);
}

char *read_file(const char *path) {
    char *buffer = NULL;
    long length;
    FILE *f = fopen(path, "rb");
    if (!f){
        fprintf(stderr, "Error: Could not open file %s\n", path);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    buffer = malloc(length);
    if (!buffer) {
        fprintf(stderr, "Error: Could not allocate memory for file %s\n", path);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    
    int k = fread(buffer, 1, length, f);
    if (k != length) {
        fprintf(stderr, "Error: Could not read file %s\n", path);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    fclose(f);

    return buffer;
}

void run_tests(){
    const char *fileName = "tests.csv";
    if (!file_exists(fileName)) {
        fprintf(stderr, "Error: Could not find file %s\n", fileName);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    char *csv = read_file(fileName);
    char *line = strtok(csv, "\n\r");

    while (line != NULL) {
        char *test = NULL, *expected = NULL;
        // necessary because intermediary strtok 
        // overwrites the original \n delimiter
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == ',') {
                line[i] = '\0';
                test = line;
                expected = line + i + 1;
                break;
            }
        }

        Tokens tokens = lex(test);
        Node *node = parse(tokens);
        double actual = evaluate_f(node);
        
        if (actual - atof(expected) > 0.0001){ // double shenanigans
            printf("Test %s failed. Expected %s, got %g.\n", test, expected, actual);
        } else {
            printf("Test %s passed.\n", test);
        }
        destroy_tree(node);
        destroy_tokens(&tokens);

        line = strtok(NULL, "\n\r");
    }
}
