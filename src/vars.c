#include "vars.h"
#include <stdio.h>

bool is_valid;

bool radians;
bool approximate;

void initialize_vars(){
    is_valid = true;
    radians = true;
    approximate = false; 
}

int set_var(char* var, char* val){
    for (int i = 0; i < VARIABLE_COUNT; i++){
        if (strcmp(calc_variables[i], var) == 0){
            switch (i){
                case 0:
                    radians = atoi(val);
                    break;
                case 1:
                    approximate = atoi(val);
                    break;
            }
            return 0;
        }
    }

    fprintf(stderr, "\nUnknown variable: %s\n", var);
    fflush(stderr);
    return 1;
}
