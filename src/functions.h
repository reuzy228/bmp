#pragma once
#include "structs.h"
#include "bmpWork.h"
void print_not_enough_or_not_valid_args();
void print_not_valid_args();
void set_pixel(Pixel_data *data, unsigned char r, unsigned char g, unsigned char b);
BMPFile* draw_line(BMPFile* bmp_file, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b);
BMPFile* draw_some_lines(BMPFile* bmp_file, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, int width);
BMPFile* mirror(BMPFile* bmp_file, Arguments *arguments);
BMPFile* copy(BMPFile* bmp_file, Arguments *arguments);
BMPFile* replace(BMPFile* bmp_file, Arguments *arguments);
BMPFile* divide(BMPFile* bmp_file, Arguments *arguments);