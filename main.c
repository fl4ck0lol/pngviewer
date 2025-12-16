#include "raylib.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<assert.h>


typedef unsigned char BYTE;
#define LENGTH 4
#define CHUNKTYPE 4
#define CRC 4

#define CNVRT32(buff) (((uint32_t)(buff)[0] << 24) | ((uint32_t)(buff)[1] << 16) |\
                      ((uint32_t)(buff)[2] << 8)  | ((uint32_t)(buff)[3]))

typedef struct
{
    int R, G, B, A;
}PIXEL;

typedef struct
{
    size_t img_width, img_height;
    FILE* img_fp;
    PIXEL** pixel_array;
}IMAGE;

void GetImgData(IMAGE* img)
{
    fseek(img->img_fp, 8, SEEK_CUR);

    size_t dataLength;

    unsigned char buff[LENGTH];

    if(fread(buff, 1, 4, img->img_fp) != 4) exit(0);

    dataLength = CNVRT32(buff);

    printf("\n[chunk data size: %d]\n", dataLength);

    unsigned char* dataBuff = (char*)malloc(sizeof(unsigned char) * dataLength);

    if(fread(buff, 1, 4, img->img_fp) != 4) exit(0);

    assert(buff[0] == 'I' && buff[1] =='H' && buff[2] == 'D' && buff[3] == 'R');

    if(fread(dataBuff, 1, dataLength, img->img_fp) != dataLength) exit(0);

    img->img_width = (size_t)(uint32_t)CNVRT32(dataBuff);

    img->img_height = (size_t)(uint32_t)CNVRT32(dataBuff + 4);

    img->pixel_array = (PIXEL**)malloc(sizeof(PIXEL*) * img->img_height);
    for(int i = 0; i < img->img_height; i++)
        img->pixel_array[i] = (PIXEL*)malloc(sizeof(PIXEL) * img->img_width);

    printf("\n[width: %zu, height: %zu]\n", img->img_width, img->img_height);

    fseek(img->img_fp, 4, SEEK_CUR);

    while(1)
    {
        if(fread(buff, 1, 4, img->img_fp) != 4) exit(0);

        dataLength = CNVRT32(buff);

        if(fread(buff, 1, 4, img->img_fp) != 4) exit(0);

        printf("\n[chunk type: %c%c%c%c][chunk data size: %d]\n", buff[0], buff[1], buff[2], buff[3], dataLength);

        if(buff[0] == 'I' && buff[1] == 'E' && buff[2] == 'N' && buff[3] == 'D')
            break;

        fseek(img->img_fp, dataLength + 4, SEEK_CUR);
    }

    printf("\n[%d]\n", dataLength);

    dataBuff = (unsigned char*)realloc(dataBuff, sizeof(unsigned char) * dataLength);

    free(dataBuff);
}

IMAGE* InitImgLoader(const char* path)
{
    IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));

    img->img_fp = fopen(path, "rb");

    assert(img->img_fp);

    GetImgData(img);

    return img;
}

int main(int argc, char** argv) 
{
    if(argc <= 1)
        exit(1);

    IMAGE* img = InitImgLoader(argv[1]);

    InitWindow(img->img_width, img->img_height, "PNG Viewer");

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    fclose(img->img_fp);

    return 0;
}
