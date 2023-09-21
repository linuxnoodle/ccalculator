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
} TEXT_ENUM; 

typedef struct {
    TEXT_ENUM text;
    void **parameters; // is really a Node**, but it's technically a circular dependency
    size_t param_count;
    char *str;
} function;

#define KEYWORD_COUNT 6
// max keyword name length is 15
__attribute__((__used__))
static char text_names[KEYWORD_COUNT][16] = {
    "sin", // sine
    "cos", // cosine 
    "tan", // tangent
    "ln", // natural log
    "exp", // e^x
    "sqrt", // square root
    "var", // used to initialize a variable
    "set_env", // used to set program variable
};
extern TEXT_ENUM hash_table[384];

void initialize_table();
size_t hash(char *str);
char *get_key(TEXT_ENUM t);

#endif /* end of include guard: HASH */
