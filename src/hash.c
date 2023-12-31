#include "hash.h"

TEXT_ENUM hash_table[384] = {0};
variable **variables;
size_t variable_count;

void initialize_table(){
    memset(hash_table, 0, 384 * sizeof(TEXT_ENUM));
    for (int i = 0; i < KEYWORD_COUNT; ++i){
        // set to enum 
        hash_table[
            hash(text_names[i])
        ] = i + 1;
    }
}

// hash function
// djb2
size_t hash(char *str){
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % 384;
}

char *get_key(TEXT_ENUM t){
    return text_names[t - 1];
}

// variable stuff (i'm not making a seperate hash table for this lmao)
void initialize_variables(){
    variables = NULL;
    variable_count = 0;
}

void add_variable(variable *v){
    if (variable_count == 0)
        variables = malloc(sizeof(variable));
    else
        variables = realloc(variables, sizeof(variable) * variable_count);
    variables[variable_count] = v;
    ++variable_count;
}

bool check_if_variable(char *name){
    for (size_t i = 0; i < variable_count; ++i){
        if (!strcmp(variables[i]->name, name)){
            return true;
        }
    }    
    return false;
}

char* get_contents(char *name){
    for (size_t i = 0; i < variable_count; ++i){
        if (!strcmp(variables[i]->name, name))
            return variables[i]->value;
    }
    return NULL;
}

void reassign_variable(char *name, char *value){
    for (size_t i = 0; i < variable_count; ++i){
        if (!strcmp(variables[i]->name, name))
            variables[i]->value = value;
    }
}
