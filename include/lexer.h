#ifndef LEXER
#define LEXER

#include "common.h"
#include "hash.h" // for functions
#include <stdbool.h>

typedef enum {
    TOKEN_FALLBACK = 0,
    TOKEN_NUMBER = 1,
    TOKEN_PLUS = 2,
    TOKEN_MINUS = 3,
    TOKEN_MULTIPLY = 4,
    TOKEN_DIVIDE = 5,
    TOKEN_POW = 6,
    TOKEN_LPAREN = 7,
    TOKEN_RPAREN = 8,
    TOKEN_COMMA = 9,
    TOKEN_TEXT = 10,
} TOKEN_TYPE;

typedef struct {
    function func;
    TOKEN_TYPE type;
    char *contents;
    size_t length;
} Token;

typedef struct {
    Token *tokens;
    size_t length;
} Tokens;

bool check_token_validity(Tokens *tokens);
void destroy_tokens(Tokens *tokens);
Tokens lex(char *in);

#endif /* end of include guard: LEXER */
