#include "engine_types.h"
#include <stdio.h>
#include <stdlib.h>

// Cria uma imagem através do tamanho e pixels recebidos
Image create_image(int size, Pixel pixels[]) {
    Image image;
    image.size = size;
    for (int i = 0; i < size; i++) {
        image.pixels[i] = pixels[i];
    }
    return image;
}

// Carrega uma imagem de um ficheiro do tipo .sprite (recebe o tamanho da imagem, coordenadas e cor dos pixels)
Image load_image_from_file(char path[]){
    Image result;

    FILE* file = fopen(path, "r");

    int imageSize = 0;
    if(!fscanf(file, "%d\n",&imageSize)){
        return result;
    }

    Pixel pixels[imageSize];

    if (NULL == file) {
        return result;
    }

    int i = 0;
    do {
        Pixel pixel;
        Vector2D pPos = { 0, 0 };
        pixel.position = pPos;
        if(!fscanf(file, "(%d,%d), %hi\n", &pixel.position.x, &pixel.position.y, &pixel.color)){
            return result;
        }
        // A palette customizada só começa nas cores acima de 8 (as anteriores são restritas do ncurses)
        pixel.color += 8;
        pixels[i] = pixel;
        i++;
    } while(!feof(file));

    result = create_image(imageSize, pixels);

    fclose(file);

    return result;
}