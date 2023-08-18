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

#define KEYWORD_COUNT 6
// max keyword name length is 15
__attribute__((__used__))
static char text_names[KEYWORD_COUNT][16] = {
    "sin",
    "cos",
    "tan",
    "ln",
    "exp",
    "sqrt",
};
extern TEXT_ENUM hash_table[128];

void initialize_table();
size_t hash(char *str);
char *get_key(TEXT_ENUM t);

#endif /* end of include guard: HASH */
