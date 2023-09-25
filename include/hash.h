#ifndef HASH
#define HASH

#include "common.h"

typedef enum {
    FALLBACK = 0,
    SIN = 1,
    COS = 2,
    TAN = 3,
    LN = 4,
    EXP = 5,
    SQRT = 6,
    FACTORIAL = 7,
    NCR = 8,
    NPR = 9,
    SET = 10,
    SEC = 11,
    CSC = 12,
    COT = 13,
    ASIN = 14,
    ACOS = 15,
    ATAN = 16,
    ACSC = 17,
    ASEC = 18,
    ACOT = 19,
    SINH = 20,
    COSH = 21,
    TANH = 22,
    ASINH = 23,
    ACOSH = 24,
    ATANH = 25,
} TEXT_ENUM; 

typedef struct {
    TEXT_ENUM text;
    void **parameters; // is really a Node**, but it's technically a circular dependency
    size_t param_count;
    char *str;
} function;

#define KEYWORD_COUNT 25
// max keyword name length is 15
__attribute__((__used__))
static char text_names[KEYWORD_COUNT][16] = {
    "sin", // sine
    "cos", // cosine 
    "tan", // tangent
    "ln", // natural log
    "exp", // e^x
    "sqrt", // square root
    "factorial", // factorial
    "ncr", // nCr
    "npr", // nPr
    "set", // used to set program variable
    "sec", // secant
    "csc", // cosecant
    "cot", // cotangent
    "asin", // arcsine
    "acos", // arccosine
    "atan", // arctangent
    "acsc", // arccosecant
    "asec", // arcsecant
    "acot", // arccotangent
    "sinh", // hyperbolic sine
    "cosh", // hyperbolic cosine
    "tanh", // hyperbolic tangent
    "asinh", // hyperbolic arcsine
    "acosh", // hyperbolic arccosine
    "atanh", // hyperbolic arctangent
};
extern TEXT_ENUM hash_table[384];

typedef struct {
    char *name, *value;
    bool defined;
} variable;

extern variable **variables;
extern size_t variable_count;

void initialize_table();
size_t hash(char *str);
char *get_key(TEXT_ENUM t);

void initialize_variables();
void add_variable(variable *v);
bool check_if_variable(char *name);
char* get_contents(char *name);
void reassign_variable(char *name, char *value);

#endif /* end of include guard: HASH */
