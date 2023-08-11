#include "eval.h"

float evaluatef(Node *tree){
    if (tree->left == NULL && tree->right == NULL){
        return atof(tree->t->contents);
    }
    float left = evaluatef(tree->left);
    float right = evaluatef(tree->right);
    switch (tree->t->type){
        case TOKEN_PLUS:
            //printf("Adding %f and %f\n", left, right);
            return left + right;
        case TOKEN_MINUS:
            //printf("Subtracting %f and %f\n", left, right);
            return left - right;
        case TOKEN_MULTIPLY:
            //printf("Multiplying %f and %f\n", left, right);
            return left * right;
        case TOKEN_DIVIDE:
            //printf("Dividing %f and %f\n", left, right);
            return left / right;
        default:
            return 0;
    }
}
