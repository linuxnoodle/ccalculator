#include "hash.h"

TEXT_ENUM hash_table[384] = {0};

void initialize_table(){
    memset(hash_table, 0, 384 * sizeof(TEXT_ENUM));
    for (int i = 0; i < KEYWORD_COUNT; ++i){
        // set to enum 
        hash_table[
            hash(text_names[i])
        ] = i + 1;
    }
}

variable *variables;
size_t variable_count, variable_capacity;
size_t* variable_hash_table;
void initialize_variable_table(){
    variable_count = 0;
    variables = malloc(sizeof(variable));
    variable_hash_table = malloc(sizeof(size_t));
    memset(hash_table, 0, sizeof(size_t));
}

void update_variable_table(){
    for (int i = 0; i < variable_count; ++i){
        size_t max_size = strlen(variables[i].name);
        if (max_size > variable_capacity){
            variable_capacity = max_size;
            variable_hash_table = realloc(variable_hash_table, sizeof(size_t) * variable_count);
        }
            
        size_t s = variable_hash(variables[i].name);
        variable_hash_table[s] = i + 1;
        hash_table[s] = i + 1;
    }
}

size_t hash(char *str){
    // first two and last character
    return *str + *(str + 1) + *(str + strlen(str) - 1);
}

size_t variable_hash(char *str){
    // sum of all characters
    size_t s = 0;
    for (size_t i = 0; i < strlen(str); ++i){
        s += *(str + i);
    }
    return s;
}

char *get_key(TEXT_ENUM t){
    return text_names[t - 1];
}
