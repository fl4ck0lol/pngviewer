#include "raylib.h"
#include<stdio.h>
#include<stdlib.h>

typedef unsigned char BYTE;

typedef struct
{
    BYTE R, G, B;
}PIXEL;

typedef struct
{
    size_t img_width, img_height;
    FILE* img_fp;
    PIXEL** pixel_array;
}IMAGE;

void GetImgData(IMAGE* img)
{

}

IMAGE* InitImgLoader(const char* path)
{
    IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));

    img->img_fp = fopen(path, "r");

    img->pixel_array = (PIXEL**)malloc(sizeof(PIXEL*) * img->img_width);

    for(int i = 0; i < img->img_width; i++)
        img->pixel_array[i] = (PIXEL*)malloc(sizeof(PIXEL) * img->img_height);

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
    return 0;
}
