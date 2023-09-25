#include "eval.h"
#include "parser.h"
#include "vars.h"
#include <stdio.h>

/* ================== BEGIN STATS FUNCTIONS ================== */

double factorial(int input){
    double d = 1;
    for (int i = 1; i <= input; i++){
        d *= i;
    }
    return d;
}

double ncr(double n, double r){
    return factorial(n) / (factorial(r) * factorial(n - r));
}

double npr(double n, double r){
    return factorial(n) / factorial(n - r);
}

/* ================== END STATS FUNCTIONS ================== */

double lookup(TEXT_ENUM type, Node** params, size_t param_count){
    // get first parameter
    if (type == FALLBACK){
        // not gonna do anything here, variables are handled in the parser
        return 0;
    }

    if (!params){
        printf("type: %d\n", type);
        fprintf(stderr, "\nError: No parameters provided to function.\n");
        fflush(stderr);
        is_valid = false;
        return 0;
    }
    double *evaluated_params = malloc(sizeof(double) * param_count);
    for (int i = 0; i < param_count; i++){
        evaluated_params[i] = evaluate_f(params[i]);
    }

    #define RADIAN_TO_DEGREE PI / 180.0
    switch (type){ // technically needs to free params but whatever
        // functions
        case SIN:
            if (param_count != 1){
                fprintf(stderr, "\nError: SIN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            
            return sin(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case COS: 
            if (param_count != 1){
                fprintf(stderr, "\nError: COS requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
                
            return cos(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case TAN:
            if (param_count != 1){
                fprintf(stderr, "\nError: TAN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            return tan(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case LN:
            if (param_count != 1){
                fprintf(stderr, "\nError: LN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] <= 0){
                fprintf(stderr, "\nError: Cannot take log of non-positive number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            
            return log(evaluated_params[0]);
        case EXP:
            return exp(evaluated_params[0]);
        case SQRT:
            if (param_count != 1){
                fprintf(stderr, "\nError: SQRT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0){
                fprintf(stderr, "\nError: Cannot take square root of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            
            return pow(evaluated_params[0], 0.5);
        case FACTORIAL:
            if (param_count != 1){
                fprintf(stderr, "\nError: FACTORIAL requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0){
                fprintf(stderr, "\nError: Cannot take factorial of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return factorial((int)evaluated_params[0]);
        case NPR:
            if (param_count != 2){
                fprintf(stderr, "\nError: NPR requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            if (evaluated_params[0] < 0 || evaluated_params[1] < 0){
                fprintf(stderr, "\nError: Cannot take NPR of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return npr(evaluated_params[0], evaluated_params[1]);
        case NCR:
            if (param_count != 2){
                fprintf(stderr, "\nError: NCR requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0 || evaluated_params[1] < 0){
                fprintf(stderr, "\nError: Cannot take NCR of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return ncr(evaluated_params[0], evaluated_params[1]);
        case SEC: 
            if (param_count != 1){
                fprintf(stderr, "\nError: SEC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return 1 / cos(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case CSC:
            if (param_count != 1){
                fprintf(stderr, "\nError: CSC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return 1 / sin(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case COT:
            if (param_count != 1){
                fprintf(stderr, "\nError: COT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return 1 / tan(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case ASIN:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASIN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return asin(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case ACOS:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOS requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return acos(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case ATAN:
            if (param_count != 1){
                fprintf(stderr, "\nError: ATAN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return atan(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case SINH:
            if (param_count != 1){
                fprintf(stderr, "\nError: SINH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return sinh(evaluated_params[0]);
        case COSH:
            if (param_count != 1){
                fprintf(stderr, "\nError: COSH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return cosh(evaluated_params[0]);
        case TANH:
            if (param_count != 1){
                fprintf(stderr, "\nError: TANH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return tanh(evaluated_params[0]);
        case ASINH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASINH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return asinh(evaluated_params[0]);
        case ACOSH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOSH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return acosh(evaluated_params[0]);
        case ATANH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ATANH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return atanh(evaluated_params[0]);
        case ASEC:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASEC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return acos(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case ACSC:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACSC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return asin(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case ACOT:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return atan(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE);
        case SET:
            if (param_count != 2){
                fprintf(stderr, "\nError: SET requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            if (!set_var(params[0]->t->contents, params[1]->t->contents))
                printf("\nSet environment variable '%s' to %s\n", params[0]->t->contents, params[1]->t->contents);
            is_valid = false;
            break;
        // constants
        default:
            fprintf(stderr, "Error: Invalid name\n");
            fflush(stderr);
            is_valid = false;
            return 0; 
    }
    return 0; // def won't cause any problems
}

char *ftoa(double f){
    size_t len = snprintf(NULL, 0, "%f", f);
    char *res = malloc(len + 1);
    snprintf(res, len + 1, "%f", f);
    return res;
}   

char *lookup_exact(TEXT_ENUM type, Node** params, size_t param_count){
    if (type == FALLBACK){
        return NULL;
    }

    if (!params){
        printf("type: %d\n", type);
        fprintf(stderr, "\nError: No parameters provided to function.\n");
        fflush(stderr);
        is_valid = false;
        return NULL;
    }
    double *evaluated_params = malloc(sizeof(double) * param_count);
    for (int i = 0; i < param_count; i++){
        evaluated_params[i] = evaluate_f(params[i]);
    }

    #define RADIANS_TO_DEGREES PI / 180.0
    switch (type){ // technically needs to free params but whatever
        case SIN:{
            if (param_count != 1){
                fprintf(stderr, "\nError: SIN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            return ftoa(sin(evaluated_params[0] * ((radians) ? 1 : RADIANS_TO_DEGREES)));
        }
        case COS: {
            if (param_count != 1){
                fprintf(stderr, "\nError: COS requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            return ftoa(cos(evaluated_params[0] * ((radians) ? 1 : RADIANS_TO_DEGREES)));
        }
        case TAN:{
            if (param_count != 1){
                fprintf(stderr, "\nError: TAN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            return ftoa(tan(evaluated_params[0] * ((radians) ? 1 : RADIANS_TO_DEGREES)));
        }
        case LN:{
            if (param_count != 1){
                fprintf(stderr, "\nError: LN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }

            if (evaluated_params[0] <= 0){
                fprintf(stderr, "\nError: Cannot take log of non-positive number.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            
            return ftoa(log(evaluated_params[0]));
        }
        case EXP:{
            return ftoa(exp(evaluated_params[0]));
        }
        case SQRT:
            if (param_count != 1){
                fprintf(stderr, "\nError: SQRT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0){
                fprintf(stderr, "\nError: Cannot take square root of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            
            return ftoa(sqrt(evaluated_params[0]));
        case FACTORIAL:
            if (param_count != 1){
                fprintf(stderr, "\nError: FACTORIAL requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0){
                fprintf(stderr, "\nError: Cannot take factorial of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return ftoa(factorial(evaluated_params[0]));
        case NPR:
            if (param_count != 2){
                fprintf(stderr, "\nError: NPR requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            if (evaluated_params[0] < 0 || evaluated_params[1] < 0){
                fprintf(stderr, "\nError: Cannot take NPR of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return ftoa(npr(evaluated_params[0], evaluated_params[1]));
        case NCR:
            if (param_count != 2){
                fprintf(stderr, "\nError: NCR requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            if (evaluated_params[0] < 0 || evaluated_params[1] < 0){
                fprintf(stderr, "\nError: Cannot take NCR of negative number.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            } 
            
            return ftoa(ncr(evaluated_params[0], evaluated_params[1]));
        case SEC: 
            if (param_count != 1){
                fprintf(stderr, "\nError: SEC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(1 / cos(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case CSC:
            if (param_count != 1){
                fprintf(stderr, "\nError: CSC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(1 / sin(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case COT:
            if (param_count != 1){
                fprintf(stderr, "\nError: COT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(1 / tan(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case ASIN:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASIN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(asin(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case ACOS:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOS requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(acos(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case ATAN:
            if (param_count != 1){
                fprintf(stderr, "\nError: ATAN requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(atan(evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case SINH:
            if (param_count != 1){
                fprintf(stderr, "\nError: SINH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(sinh(evaluated_params[0]));
        case COSH:
            if (param_count != 1){
                fprintf(stderr, "\nError: COSH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(cosh(evaluated_params[0]));
        case TANH:
            if (param_count != 1){
                fprintf(stderr, "\nError: TANH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(tanh(evaluated_params[0]));
        case ASINH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASINH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(asinh(evaluated_params[0]));
        case ACOSH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOSH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(acosh(evaluated_params[0]));
        case ATANH:
            if (param_count != 1){
                fprintf(stderr, "\nError: ATANH requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(atanh(evaluated_params[0]));
        case ASEC:
            if (param_count != 1){
                fprintf(stderr, "\nError: ASEC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(acos(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case ACSC:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACSC requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(asin(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case ACOT:
            if (param_count != 1){
                fprintf(stderr, "\nError: ACOT requires one parameter.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            return ftoa(atan(1 / evaluated_params[0]) * ((radians) ? 1 : RADIAN_TO_DEGREE));
        case SET:
            if (param_count != 2){
                fprintf(stderr, "\nError: SET requires two parameters.\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }
            if (!set_var(params[0]->t->contents, params[1]->t->contents))
                printf("\nSet environment variable '%s' to %s\n", params[0]->t->contents, params[1]->t->contents);
            is_valid = false;
            break;
        default:
            fprintf(stderr, "Error: Invalid name\n");
            fflush(stderr);
            is_valid = false;
            return 0; 
    }
    return 0;
}

double evaluate_f(Node *tree){
    if (variables == NULL){
        variables = malloc(sizeof(variable));
    }

    double left = 0, right = 0;
    if (tree && !tree->left && !tree->right){
        if (tree->t->type != TOKEN_TEXT)
            return atof(tree->t->contents);
    } else if (tree && tree->left && tree->right){
        left = evaluate_f(tree->left);
        right = evaluate_f(tree->right);
    } else if (!tree){
        fprintf(stderr, "Error: Invalid syntax tree\n");
        fflush(stderr);
        is_valid = false;
        return 0;
    }

    switch (tree->t->type){
        case TOKEN_PLUS:
            return left + right;
        case TOKEN_MINUS:
            return left - right;
        case TOKEN_MULTIPLY:
            return left * right;
        case TOKEN_DIVIDE:
            return left / right;
        case TOKEN_POW:
            return pow(left, right);
        case TOKEN_TEXT:
            return lookup(tree->t->func.text,
                          (Node**)tree->t->func.parameters,
                          tree->t->func.param_count);
        case TOKEN_EQUALS:
            // make sure left side is only a string
            if (tree->left->t->type != TOKEN_TEXT){
                fprintf(stderr, "Error: Invalid syntax tree\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            // also make sure there's nothing but text, so left should have no children
            } else if (tree->left->left || tree->left->right){
                fprintf(stderr, "Error: Invalid syntax tree\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            int len = snprintf(NULL, 0, "%f", right);
            char *s = malloc(len + 1);
            snprintf(s, len + 1, "%f", right);
            // check if variable already exists
            if (check_if_variable(tree->left->t->contents)){
                reassign_variable(tree->left->t->contents, s);
                is_valid = false;
                printf("%s = %s\n", tree->left->t->contents, s);
            } else {
                // otherwise make a new variable
                variable *v = malloc(sizeof(variable));
                char* name = malloc(strlen(tree->left->t->contents) + 1);
                strcpy(name, tree->left->t->contents);
                
                v->name = name; // preventing freeing when tree is freed
                v->value = s;
                add_variable(v);
                is_valid = false;
                printf("%s = %s\n", tree->left->t->contents, s);
            }
            return 0;
        default:
            fprintf(stderr, "Error: Invalid token type\n");
            fflush(stderr);
            is_valid = false;
            return 0;
    }
}

char *evaluate_exact(Node* tree){
    if (variables == NULL){
        variables = malloc(sizeof(variable));
    }

    char *left = NULL, *right = NULL;
    if (tree && !tree->left && !tree->right){
        if (tree->t->type != TOKEN_TEXT)
            return tree->t->contents;
    } else if (tree && tree->left && tree->right){
        left = evaluate_exact(tree->left);
        right = evaluate_exact(tree->right);
    } else if (!tree){
        fprintf(stderr, "Error: Invalid syntax tree\n");
        fflush(stderr);
        is_valid = false;
        return NULL;
    }

    switch (tree->t->type){
        case TOKEN_PLUS:
            // make sure there's both a left and right
            if (!left || !right){
                fprintf(stderr, "Error: Invalid syntax tree. Make sure you provide a left and right argument to +.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            // if both are numeric, add them
            // if one is a variable then just concatenate
            if (tree->left->t->type == TOKEN_NUMBER && tree->right->t->type == TOKEN_NUMBER){
                char *s = malloc(strlen(left) + strlen(right) + 3); // number can't be larger than the sum of both of them plus 1
                double sum = atof(left) + atof(right);
                snprintf(s, strlen(left) + strlen(right) + 3, "%g", sum);
                return s;
            // both are TOKEN_TEXT and both aren't functions (check in hash table)
            } else if (tree->left->t->type == TOKEN_TEXT && tree->right->t->type == TOKEN_TEXT){
                TEXT_ENUM text_left = hash_table[hash(tree->left->t->contents)],
                          text_right = hash_table[hash(tree->right->t->contents)];
                // checking if either are non-functions
                if ((text_left == 0 || text_right == 0) || (strcmp(tree->left->t->contents, get_key(text_left)) || (strcmp(tree->right->t->contents, get_key(text_right))))){
                    // check if both are the same string, if so add coefficients
                    if (!strcmp(tree->left->t->contents, tree->right->t->contents)){
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "2 * %s", left);
                        return s;
                    } else {
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "%s + %s", left, right);
                        return s;
                    }
                } else {
                    char *s = malloc(strlen(left) + strlen(right) + 3);
                    snprintf(s, strlen(left) + strlen(right) + 3, "%s + %s", left, right);
                    return s;
                }
            // literally just concatenate both 
            } else {
                char *s = malloc(strlen(left) + strlen(right) + 1);
                strcat(s, left);
                strcat(s, " + ");
                strcat(s, right);
                return s;
            }
        case TOKEN_MINUS:
            if (!left || !right){
                fprintf(stderr, "Error: Invalid syntax tree. Make sure you provide a left and right argument to +.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            if (tree->left->t->type == TOKEN_NUMBER && tree->right->t->type == TOKEN_NUMBER){
                char *s = malloc(strlen(left) + strlen(right) + 3); 
                double diff = atof(left) - atof(right);
                snprintf(s, strlen(left) + strlen(right) + 3, "%g", diff);
                return s;
            } else if (tree->left->t->type == TOKEN_TEXT && tree->right->t->type == TOKEN_TEXT){
                TEXT_ENUM text_left = hash_table[hash(tree->left->t->contents)],
                          text_right = hash_table[hash(tree->right->t->contents)];
                if ((text_left == 0 || text_right == 0) || (strcmp(tree->left->t->contents, get_key(text_left)) || (strcmp(tree->right->t->contents, get_key(text_right))))){
                    if (!strcmp(tree->left->t->contents, tree->right->t->contents)){
                        char *s = malloc(2);
                        snprintf(s, 2, "0");
                        return s;
                    } else {
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "%s - %s", left, right);
                        return s;
                    }
                } else {
                    char *s = malloc(strlen(left) + strlen(right) + 3);
                    snprintf(s, strlen(left) + strlen(right) + 3, "%s - %s", left, right);
                    return s;
                }
            } else {
                char *s = malloc(strlen(left) + strlen(right) + 3);
                strcat(s, left);
                strcat(s, " - ");
                strcat(s, right);
                return s;
            }
        case TOKEN_MULTIPLY:
            if (!left || !right){
                fprintf(stderr, "Error: Invalid syntax tree. Make sure you provide a left and right argument to +.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            if (tree->left->t->type == TOKEN_NUMBER && tree->right->t->type == TOKEN_NUMBER){
                char *s = malloc(strlen(left) + strlen(right) + 1); 
                double times = atof(left) * atof(right);
                snprintf(s, strlen(left) + strlen(right) + 1, "%g", times);
                return s;
            } else if (tree->left->t->type == TOKEN_TEXT && tree->right->t->type == TOKEN_TEXT){
                TEXT_ENUM text_left = hash_table[hash(tree->left->t->contents)],
                          text_right = hash_table[hash(tree->right->t->contents)];
                if ((text_left == 0 || text_right == 0) || (strcmp(tree->left->t->contents, get_key(text_left)) || (strcmp(tree->right->t->contents, get_key(text_right))))){
                    if (!strcmp(tree->left->t->contents, tree->right->t->contents)){
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "%s ^ %s", left, right);
                        return s;
                    } else {
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "%s * %s", left, right);
                        return s;
                    }
                } else {
                    char *s = malloc(strlen(left) + strlen(right) + 3);
                    snprintf(s, strlen(left) + strlen(right) + 3, "%s * %s", left, right);
                    return s;
                }
            } else {
                char *s = malloc(strlen(left) + strlen(right) + 3);
                strcat(s, left);
                strcat(s, " * ");
                strcat(s, right);
                return s;
            }
        case TOKEN_DIVIDE:
            if (!left || !right){
                fprintf(stderr, "Error: Invalid syntax tree. Make sure you provide a left and right argument to +.\n");
                fflush(stderr);
                is_valid = false;
                return NULL;
            }
            if (tree->left->t->type == TOKEN_NUMBER && tree->right->t->type == TOKEN_NUMBER){
                char *s = malloc(strlen(left) + strlen(right) + 1); 
                double times = atof(left) / atof(right);
                snprintf(s, strlen(left) + strlen(right) + 1, "%g", times);
                return s;
            } else if (tree->left->t->type == TOKEN_TEXT && tree->right->t->type == TOKEN_TEXT){
                TEXT_ENUM text_left = hash_table[hash(tree->left->t->contents)],
                          text_right = hash_table[hash(tree->right->t->contents)];
                if ((text_left == 0 || text_right == 0) || (strcmp(tree->left->t->contents, get_key(text_left)) || (strcmp(tree->right->t->contents, get_key(text_right))))){
                    if (!strcmp(tree->left->t->contents, tree->right->t->contents)){
                        char *s = malloc(2);
                        snprintf(s, 2, "1");
                        return s;
                    } else {
                        char *s = malloc(strlen(left) + strlen(right) + 3);
                        snprintf(s, strlen(left) + strlen(right) + 3, "%s / %s", left, right);
                        return s;
                    }
                } else {
                    char *s = malloc(strlen(left) + strlen(right) + 3);
                    snprintf(s, strlen(left) + strlen(right) + 3, "%s / %s", left, right);
                    return s;
                }
            } else {
                char *s = malloc(strlen(left) + strlen(right) + 3);
                strcat(s, left);
                strcat(s, " * ");
                strcat(s, right);
            }
        case TOKEN_EQUALS:
            // make sure left side is only a string
            if (tree->left->t->type != TOKEN_TEXT){
                fprintf(stderr, "Error: Invalid syntax tree\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            // also make sure there's nothing but text, so left should have no children
            } else if (tree->left->left || tree->left->right){
                fprintf(stderr, "Error: Invalid syntax tree\n");
                fflush(stderr);
                is_valid = false;
                return 0;
            }

            char *s = malloc(strlen(right) + 1);
            strcat(s, right);
            // check if variable already exists
            if (check_if_variable(tree->left->t->contents)){
                reassign_variable(tree->left->t->contents, s);
                is_valid = false;
                printf("%s = %s\n", tree->left->t->contents, s);
            } else {
                // otherwise make a new variable
                variable *v = malloc(sizeof(variable));
                char* name = malloc(strlen(tree->left->t->contents) + 1);
                strcpy(name, tree->left->t->contents);
                
                v->name = name; // preventing freeing when tree is freed
                v->value = s;
                add_variable(v);
                is_valid = false;
                printf("%s = %s\n", tree->left->t->contents, s);
            }
            return 0;
        case TOKEN_TEXT: {
            // check if it's a function
            TEXT_ENUM text = hash_table[hash(tree->t->contents)];
            if (text != 0 && !strcmp(tree->t->contents, get_key(text))){
                return lookup_exact(tree->t->func.text, (Node**)tree->t->func.parameters, tree->t->func.param_count);
            } else {
                // just return the variable 
                char *var_name = malloc(strlen(tree->t->contents) + 3);
                strcpy(var_name, tree->t->contents);
                return var_name;
            }
        }
        default:
            printf("token type: %d\n", tree->t->type);
            return "TODO";
    }
}
