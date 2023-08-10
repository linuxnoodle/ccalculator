#include "lexer.h"
#include <string.h>

void destroy_tokens(Tokens *tokens){
    for (size_t i = 0; i < tokens->length; ++i){
        free(tokens->tokens[i].contents);
    }
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

    // TODO: make proper vector type later
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

        if (current_state != last_state && buffer_index > 0){
            tokens = (Token*) realloc(tokens, sizeof(Token) * (out.length + 1));

            tokens[out.length].type = last_state;

            tokens[out.length].contents = (char*) malloc(sizeof(char) * buffer_index + 1);
            memcpy(tokens[out.length].contents, buffer, buffer_index);
            tokens[out.length].contents[buffer_index] = '\0';
        
            tokens[out.length].length = buffer_index;

            memset(buffer, 0, 256);
            ++out.length;
            buffer_index = 0;

            buffer[buffer_index] = in[i];
            ++buffer_index;

            last_state = current_state;
        } else {
            buffer[buffer_index] = in[i];
            ++buffer_index;
        }
        last_state = current_state;
    }

    // TODO: make this more compact
    if (buffer_index > 0){
        tokens = (Token*) realloc(tokens, sizeof(Token) * (out.length + 1));
        last_state = current_state;
        
        tokens[out.length].type = current_state;
        
        tokens[out.length].contents = (char*) malloc(sizeof(char) * buffer_index + 1);
        memcpy(tokens[out.length].contents, buffer, buffer_index);
        tokens[out.length].contents[buffer_index] = '\0';
        
        tokens[out.length].length = buffer_index;
            
        memset(buffer, 0, 256);
        ++out.length;
        buffer_index = 0;
    }

    out.tokens = tokens;
    return out;
}
