#include "lexer.h"
#include <string.h>

void destroy_tokens(Tokens *tokens){
    free(tokens->tokens);
}

TOKEN_TYPE get_associated_state(char c){
    if (c >= '0' && c <= '9'){
        return TOKEN_NUMBER;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return TOKEN_TEXT;
    }
    switch (c){
        case '+':
            return TOKEN_PLUS;
        case '-':
            return TOKEN_MINUS;
        case '*':
            return TOKEN_MULTIPLY;
        case '/':
            return TOKEN_DIVIDE;
        case '(':
            return TOKEN_LPAREN;
        case ')':
            return TOKEN_RPAREN;
        default:
            return TOKEN_FALLBACK;
    }
}

// return array of tokens
Tokens lex(char *in){
    Tokens out;
    size_t len = strlen(in);

    TOKEN_TYPE current_state, last_state;
    current_state = last_state = TOKEN_FALLBACK;

    // really stupid way of dynamically allocating, make proper vector type later
    Token *tokens = (Token*) malloc(sizeof(Token));
    out.length = 0;

    char buffer[256];
    memset(buffer, 0, 256);

    size_t buffer_index = 0;
    for (size_t i = 0; i < len; ++i){
        TOKEN_TYPE tmp = get_associated_state(in[i]);
        if (tmp != TOKEN_FALLBACK){
            current_state = tmp;
        } else {
            continue;
        }

        if (current_state != last_state || i == len - 1){
            // if we're in a buffer, flush it
            if (buffer_index > 0){
                // malloc new token 
                tokens = (Token*) realloc(tokens, sizeof(Token) * (out.length + 1));
                last_state = current_state;

                tokens[out.length].type = current_state;
                tokens[out.length].contents = buffer;
                memset(buffer, 0, 256);
                ++out.length;

                buffer_index = 0;
            }
        } else {
            buffer[buffer_index] = in[i];
            ++buffer_index;
        }
        last_state = current_state;
    }

    out.tokens = tokens;
    return out;
}
