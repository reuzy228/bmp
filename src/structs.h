#pragma once
#include <getopt.h>
#include <string.h>
#pragma pack (push, 1)
typedef struct Arguments {
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int axis;
    int line_width;
    int color1_r, color1_g, color1_b;
    int color2_r, color2_g, color2_b;
    int x_count, y_count;
    char *output;
} Arguments;

typedef struct BMPHeader {
    unsigned char ID[2];
    unsigned int file_size;
    unsigned char unused[4];
    unsigned int pixel_offset;
} BMPHeader;

typedef struct DIBHeader {
    unsigned int header_size;
    unsigned int width;
    unsigned int height;
    unsigned short color_planes;
    unsigned short bits_per_pixel;
    unsigned int compression;
    unsigned int data_size;
    unsigned int pwidth;
    unsigned int pheight;
    unsigned int colors_count;
    unsigned int imp_colors_count;
} DIBHeader;

typedef struct Pixel_data {
    unsigned char red;
    unsigned char blue;
    unsigned char green;
} Pixel_data;

typedef struct BMPFile {
    BMPHeader bhdr;
    DIBHeader dhdr;
    unsigned char *data;
    Pixel_data **pixel_data;
} BMPFile;

#pragma pack(pop)