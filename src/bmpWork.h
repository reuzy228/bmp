#pragma once
#include "structs.h"

void readBMP(FILE* fp, BMPFile** bmp_file);
BMPFile* loadBMPFile(char* frame);
void freeBMPFile(BMPFile* bmp_file);
void print_bmp_info(BMPFile* bmp_file);
void print_bmp_pixels(BMPFile* bmp_file);
unsigned char* rgb_to_str(BMPFile* bmp_file);
void output_bmp_file(BMPFile* bmp_file, char* output_file);