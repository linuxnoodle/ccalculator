#ifndef VARS_H
#define VARS_H

#include "common.h"

extern bool is_valid;

// env variables
extern bool radians;
extern bool approximate;

#define VARIABLE_COUNT 2
static char* calc_variables[VARIABLE_COUNT] = {
    "radians",
    "approximate"
};

void initialize_vars();
int set_var(char* var, char* val);

#endif
