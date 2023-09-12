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

size_t hash(char *str){
    // first two and last character
    return *str + *(str + 1) + *(str + strlen(str) - 1);
}

char *get_key(TEXT_ENUM t){
    return text_names[t - 1];
}
