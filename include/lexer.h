#ifndef LEXER
#define LEXER

#include "common.h"

typedef enum {
    TOKEN_FALLBACK = 0,
    TOKEN_NUMBER = 1,
    TOKEN_PLUS = 2,
    TOKEN_MINUS = 3,
    TOKEN_MULTIPLY = 4,
    TOKEN_DIVIDE = 5,
    TOKEN_LPAREN = 6,
    TOKEN_RPAREN = 7,
    TOKEN_TEXT = 8,
} TOKEN_TYPE;

typedef struct {
    TOKEN_TYPE type;
    char *contents;
    size_t length;
} Token;

typedef struct {
    Token *tokens;
    size_t length;
} Tokens;

void destroy_tokens(Tokens *tokens);
Tokens lex(char *in);

#endif /* end of include guard: LEXER */
