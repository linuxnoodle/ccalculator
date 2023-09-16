#ifndef EVAL
#define EVAL

#include "common.h"
#include "parser.h"

#define PI 3.14159265358979323846 
#define LN_2 0.693147180559945309417232121458176568
#define LOG2_E 1.44269504088896
#define LOG2_10 3.32192809488736

double evaluate_f(Node *tree);
char *evaluate_exact(Node *tree);

#endif /* end of include guard: EVAL */
