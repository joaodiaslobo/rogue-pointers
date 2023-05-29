#include "utils.h"
#include <stdlib.h>

/*

* a104356 - João Lobo

* Retorna o número de caracteres newline numa string.

*/
int count_newlines(char text[]){
    int count = 0;
    for(int i = 0; text[i] != '\0'; i++){
        if(text[i] == '\n'){
            count++;
        }
    }
    return count;
}