#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

int main(){
    initialize_table();
    while (1) {
        printf("> ");

        char buffer[256];
        if (scanf("%[^\n]%*c", buffer) == 0){
            printf("Error: scanf did an oopsie woopsie\n");
        }

        Tokens t = lex(buffer);
        Node *tree = form_tree(&t);

        /*print_tree(tree, 0);
        printf("\n");*/
        
        double res = evaluate_f(tree);

        printf("%s = %g\n", buffer, res);

        destroy_tokens(&t);
        destroy_tree(tree);
    }
    return 0;
}
