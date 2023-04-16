#include <stdlib.h>

// Conta o número de caracteres newline numa string
int count_newlines(char text[]){
    int count = 0;
    for(int i = 0; text[i] != '\0'; i++){
        if(text[i] == '\n'){
            count++;
        }
    }
    return count;
}

// Encontra a posição do próximo caracter newline (quando não encontra retorna o fim da string)
int get_next_newline_position(char text[], int startPos){
    int i = startPos;
    for(i = startPos; text[i] != '\n' && text[i] != '\0'; i++);
    return i;
}

// Retorna uma substring de uma string, recebe a string inicial, a posição inicial do "corte" e posição final
char* substring(char source[], int startPos, int endPos) {
    int size = endPos - startPos + 1;
    char* result = (char*)malloc((size + 1) * sizeof(char));
    int i;
    for (i = startPos; i <= endPos; i++) {
        result[i - startPos] = source[i];
    }
    result[i - startPos] = '\0';
    return result; 
}