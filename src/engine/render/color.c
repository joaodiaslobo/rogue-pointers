#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "engine_types.h"

int two_hexadecimal_to_int(char hex[]);
Color hex_to_color(char hex[]);
int add_color_to_palette(Color color, short index);
int load_palette_from_file(char *path);

// Carrega um ficheiro palette e adicona as cores à memória do ncurses, a partir do index de cores 8
int load_palette_from_file(char *path){
    FILE* file = fopen(path, "r");

    if (NULL == file) {
        return 0;
    }

    char ch;
    char hex[8];
    int index = 0;
    short colorIndex = 0;
    
    do {
        ch = fgetc(file);
        if(ch != EOF){
            if(ch != '\n'){
                hex[index] = ch;
                index++;
            } else {
                add_color_to_palette(hex_to_color(hex), 8 + colorIndex);
                index = 0;
                colorIndex++;
            }
        }
    } while (ch != EOF);
    
    fclose(file);
    return 1;
}

// Adiciona cor à lista de cores do ncurses
int add_color_to_palette(Color color, short index){
    short red = (color.red * 1000.0) / 255.0;
    short green = (color.green * 1000.0) / 255.0; 
    short blue = (color.blue * 1000.0) / 255.0;
    init_color(index, red, green, blue);
    return 0;
}

// Passa um valor hexadecimal para uma struct cor
Color hex_to_color(char hex[]){
    Color color;
    char red[] = {hex[1],hex[2]};
    char green[] = {hex[3],hex[4]};
    char blue[] = {hex[5],hex[6]};
    color.red = two_hexadecimal_to_int(red);
    color.green = two_hexadecimal_to_int(green);
    color.blue = two_hexadecimal_to_int(blue);
    return color;
}

// Utilitário para passar valores hexadecimais de dois dígitos com letras maiusculas para um inteiro
int two_hexadecimal_to_int(char hex[]){
   
    int result = 0;

    if (hex[0] >= 65){
        result += ((hex[0] - 55) * 16);
    } else {
        result += ((hex[0] - 48)  * 16);
    }

    if (hex[1] >= 65){
        result += (hex[1] - 55);
    } else {
        result += (hex[1] - 48);
    }

    return result;
}