#include "lexer.h"

void destroy_tokens(Tokens *tokens){
    for (size_t i = 0; i < tokens->length; ++i){
        free(tokens->tokens[i].contents);
    }
    free(tokens->tokens);
}

TOKEN_TYPE get_associated_state(char c){
    if ((c >= '0' && c <= '9') || c == '.'){
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
        case '^':
            return TOKEN_POW;
        case ',':
            return TOKEN_COMMA;
        case '=':
            return TOKEN_EQUALS;
        default:
            return TOKEN_FALLBACK;
    }
}

bool check_token_validity(Tokens *tokens){
    if (tokens->length < 2){
        return false;
    }
    for (size_t i = 0; i < tokens->length - 1; ++i){
        if (tokens->tokens[i].type == TOKEN_NUMBER
        && tokens->tokens[i + 1].type == TOKEN_NUMBER){
            fprintf(stderr, "Error: You probably forgot to add an operator(s) or comma(s).\n");
            fflush(stderr);
            return false;
        }
    }

    // make sure every parenthesis is closed
    int paren_count = 0;
    for (size_t i = 0; i < tokens->length; ++i){
        if (tokens->tokens[i].type == TOKEN_LPAREN){
            ++paren_count;
        } else if (tokens->tokens[i].type == TOKEN_RPAREN){
            --paren_count;
        }
    }
    return true;
}

// this hurts to write
void push_buffer(char *buffer,
                 char *in,
                 Tokens *out,
                 Token *tokens,
                 size_t *buffer_index,
                 TOKEN_TYPE state,
                 size_t i){
    // realloc dies when put in a function for some reason
    tokens[out->length].type = state;

    tokens[out->length].contents = (char*) malloc(sizeof(char) * (*buffer_index) + 1);
    memcpy(tokens[out->length].contents, buffer, *buffer_index);
    tokens[out->length].contents[*buffer_index] = '\0';
    tokens[out->length].length = *buffer_index;

    memset(buffer, 0, 256);
    ++out->length;
    *buffer_index = 0;
}

// return array of tokens
Tokens lex(char *in){
    Tokens out;
    size_t len = strlen(in);

    TOKEN_TYPE current_state, last_state;
    current_state = last_state = TOKEN_FALLBACK;

    Token *tokens = malloc(sizeof(Token));
    out.length = 0;

    // TODO: allow for dynamic buffer size
    char buffer[256];
    memset(buffer, 0, 256);
    size_t buffer_index = 0;
    for (size_t i = 0; i < len; ++i){
        TOKEN_TYPE tmp = get_associated_state(in[i]);
        if (tmp != TOKEN_FALLBACK){
            current_state = tmp;
        } else if (last_state != TOKEN_FALLBACK){
            tokens = realloc(
                tokens,
                sizeof(Token) * (out.length + 1)
            );
            push_buffer(
                buffer,
                in,
                &out,
                tokens,
                &buffer_index,
                last_state, 
                i
            );
            continue;
        } else {
            continue;
        }

        if (current_state != last_state && buffer_index > 0 || 
            current_state == TOKEN_LPAREN || current_state == TOKEN_RPAREN){
            tokens = realloc(
                tokens,
                sizeof(Token) * (out.length + 1)
            );
            push_buffer(
                buffer,       
                in,        
                &out,
                tokens,
                &buffer_index,
                last_state, 
                i
            );
        }
        buffer[buffer_index] = in[i];
        ++buffer_index;
        last_state = current_state;
    }

    // TODO: make this more compact
    if (buffer_index > 0){
        tokens = realloc(
            tokens,
            sizeof(Token) * (out.length + 1)
        );
        push_buffer(
            buffer,
            in,
            &out,
            tokens,
            &buffer_index,
            last_state, 
            len
        );
    }
    out.tokens = tokens;
    return out;
}
