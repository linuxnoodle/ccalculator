#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

int main(){
    while (1) {
        printf("> ");

        char buffer[256];
        if (scanf("%[^\n]%*c", buffer) == 0){
            printf("Error: scanf did an oopsie woopsie\n");
        }

        Tokens t = lex(buffer);
        Node *tree = form_tree(&t);
        float res = evaluatef(tree);

        printf("%s = %f\n", buffer, res);

        destroy_tokens(&t);
        destroy_tree(tree);
    }
    return 0;
}
