#include "common.h"
#include "lexer.h"
#include "parser.h"

int main(){
    // example parsing 
    char* s = "14 + (22 / 22)";
    Tokens t = lex(s);
    
    printf("length: %lu\n", t.length);
    for (size_t i = 0; i < t.length; ++i){
        printf("%s: %d\n", t.tokens[i].contents, t.tokens[i].type);
    }
    printf("\n");

    destroy_tokens(&t);
    return 0;
}
