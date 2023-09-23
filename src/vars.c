#include "vars.h"
#include <stdio.h>

bool is_valid;

bool radians;
int64_t precision;

void initialize_vars(){
    is_valid = true;
    radians = true;
    precision = -1; 
}

int set_var(char* var, char* val){
    for (int i = 0; i < VARIABLE_COUNT; i++){
        if (strcmp(calc_variables[i], var) == 0){
            switch (i){
                case 0:
                    radians = atoi(val);
                    break;
                case 1:
                    // todo
                    break;
            }
            return 0;
        }
    }

    fprintf(stderr, "Unknown variable: %s\n", var);
    fflush(stderr);
    return 1;
}