#include "structs.h"
#include "headers.h"
#include "bmpWork.h"

void print_not_enough_or_not_valid_args() {
    printf("Было введено недостаточное количество аргументов либо данные не являются валидными. Работа программы завершена.\n");
    exit(-1);
}

void print_not_valid_args() {
    printf("Данные аргументов не являются валидными. Работа программы завершена.\n");
    exit(-1);
}

int check_color(int color) {
    return !(0 <= color && color <= 255);
}

void set_pixel(Pixel_data *data, unsigned char r, unsigned char g, unsigned char b) {
    data->red = r;
    data->green = g;
    data->blue = b;
}

BMPFile* draw_line(BMPFile* bmp_file, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);
    int signX = x1 < x2 ? 1 : -1;
    int signY = y1 < y2 ? 1 : -1;
    int error = dx - dy;
    int new_error;
    set_pixel(&bmp_file->pixel_data[y2][x2], r, g, b);
    while (x1 != x2 || y1 != y2) {
        set_pixel(&bmp_file->pixel_data[y1][x1], r, g, b);
        new_error = error*2;
        if (new_error > -dy) {
            error -= dy;
            x1 += signX;
        }
        if (new_error < dx) {
            error += dx;
            y1 += signY;
        }
    }
    return bmp_file;
}

BMPFile* draw_some_lines(BMPFile* bmp_file, int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, int width) {
    int singX = x1 < x2 ? 1 : -1;
    int signY = y1 < y2 ? 1 : -1;
    
    if (y2==y1) {
        int right = width / 2;
        int left = right;
        if (width % 2 == 0) {
            left -= 1;
        }

        for(int i = -left; i <= right; i++) {
            bmp_file = draw_line(bmp_file, x1, y1+i, x2, y2+i, r, g, b);
        }
    } else {
        int right = width / 2;
        int left = right;
        if (width % 2 == 0) {
            left -= 1;
        }
        for(int i = -left; i <= right; i++) {
            bmp_file = draw_line(bmp_file, x1+i, y1, x2+i, y2, r, g, b);
        }
    }

    return bmp_file;
}

BMPFile* mirror(BMPFile* bmp_file, Arguments *arguments) { 
    int *axis = &(arguments->axis);
    int *x1 = &(arguments->x1), *y1 = &(arguments->y1);
    int *x2 = &(arguments->x2), *y2 = &(arguments->y2);
    
    if (*axis == -1 || *x1 == -1  || *x2 == -1) print_not_enough_or_not_valid_args();
    if (*axis != 0 && *axis != 1) print_not_valid_args();
    if (*x1 < 0 || *x1 > (bmp_file->dhdr.width-1) || *x2 < 0 || *x2 > (bmp_file->dhdr.width-1) || *x2<=*x1) print_not_valid_args();
    if (*y1 < 0 || *y1 > (bmp_file->dhdr.height-1) || *y2 < 0 || *y2 > (bmp_file->dhdr.height-1) || *y2<=*y1) print_not_valid_args();

    Pixel_data tmp;
    int k;

    if (*axis == 0) {
        for(int i = *y1; i <= *y2; i++) {
            for(int j = *x1; j <= (*x2+*x1)/2; j++) {
                k = *x2 - (j - *x1);
                tmp = bmp_file->pixel_data[i][k];
                bmp_file->pixel_data[i][k] = bmp_file->pixel_data[i][j];
                bmp_file->pixel_data[i][j] = tmp;
            }
        }
    } else {
        for(int j = *x1; j <= *x2; j++) {
            for(int i = *y1; i <= (*y2+*y1)/2; i++) {
                k = *y2 - (i - *y1);
                tmp = bmp_file->pixel_data[k][j];
                bmp_file->pixel_data[k][j] = bmp_file->pixel_data[i][j];
                bmp_file->pixel_data[i][j] = tmp;
            }
        }
    }   

    *axis = -1, *x1 = -1, *y1 = -1, *x2 = -1, *y2 = -1;
    return bmp_file;
}

BMPFile* copy(BMPFile* bmp_file, Arguments *arguments) {
    int *x1 = &(arguments->x1), *y1 = &(arguments->y1);
    int *x2 = &(arguments->x2), *y2 = &(arguments->y2);
    int *x3 = &(arguments->x3), *y3 = &(arguments->y3);
   
    if (*x1 == -1 || *x2 == -1  || *x3 == -1) print_not_enough_or_not_valid_args();
    if (*x1 < 0 || *x1 > (bmp_file->dhdr.width-1) || *x2 < 0 || *x2 > (bmp_file->dhdr.width-1) || *x3 < 0 || *x3 > (bmp_file->dhdr.width-1)) print_not_valid_args();
    if (*y1 < 0 || *y1 > (bmp_file->dhdr.height-1) || *y2 < 0 || *y2 > (bmp_file->dhdr.height-1) || *y3 < 0 || *y3 > (bmp_file->dhdr.height-1)) print_not_valid_args();

    int dx = *x2 - *x1, dy = *y2 - *y1;
    int x4 = *x3 +  dx, y4 = *y3 +  dy; 

    if (x4 < 0 || x4 > (bmp_file->dhdr.width-1) || y4 < 0 || y4 > (bmp_file->dhdr.height-1)) print_not_valid_args();

    Pixel_data** new_data = malloc(bmp_file->dhdr.height * sizeof(Pixel_data*));
    Pixel_data** old_data = bmp_file->pixel_data;
    
    for(int i = 0; i < bmp_file->dhdr.height; i++) {
        new_data[i] = malloc(bmp_file->dhdr.width * sizeof(Pixel_data));
        for(int j = 0; j < bmp_file->dhdr.width; j++) {
            new_data[i][j] = old_data[i][j];
        }
    }

    for(int i = *y1; i <= *y2; i++) {
        for(int j = *x1; j <= *x2; j++) {
            new_data[*y3 + (i - *y1)][*x3 + (j - *x1)] = old_data[i][j];  
        }
    }

    for(int i = 0; i < bmp_file->dhdr.height; i++) {
        free(old_data[i]);
    }
    free(old_data);

    bmp_file->pixel_data = new_data;
    *x1 = -1, *x2 = -1, *x3 = -1, *y1 = -1, *y2 = -1, *y3 = -1;
    return bmp_file;

}

BMPFile* replace(BMPFile* bmp_file, Arguments *arguments) {
    int *color1_r = &(arguments->color1_r), *color1_g = &(arguments->color1_g), *color1_b = &(arguments->color1_b);
    int *color2_r = &(arguments->color2_r), *color2_g = &(arguments->color2_g), *color2_b = &(arguments->color2_b);
   
    if (*color1_r == -1 || *color2_r == -1) print_not_enough_or_not_valid_args();
    if (check_color(*color1_r) || check_color(*color1_g) || check_color(*color1_b) || check_color(*color2_r) || check_color(*color2_g) || check_color(*color2_b)) print_not_valid_args();

    unsigned char r1 = (unsigned char)*color1_r, g1 = (unsigned char)*color1_g, b1 = (unsigned char)*color1_b;
    unsigned char r2 = (unsigned char)*color2_r, g2 = (unsigned char)*color2_g, b2 = (unsigned char)*color2_b;

    Pixel_data *data;

    for(int i = 0; i < bmp_file->dhdr.height; i++) {
        for(int j = 0; j < bmp_file->dhdr.width; j++) {
            data = &bmp_file->pixel_data[i][j];
            if (data->red == r1 && data->green == g1 && data->blue == b1) {
                set_pixel(data, r2, g2, b2);
            }
        }
    }

    *color1_r = -1, *color1_g = -1, *color1_b = -1;
    *color2_r = -1, *color2_g = -1, *color2_b = -1;

    return bmp_file;
}

BMPFile* divide(BMPFile* bmp_file, Arguments *arguments) {
    int *x_count = &(arguments->x_count), *y_count = &(arguments->y_count);
    int *line_width = &(arguments->line_width);
    int *color1_r = &(arguments->color1_r), *color1_g = &(arguments->color1_g), *color1_b = &(arguments->color1_b);
    
    if (check_color(*color1_r) || check_color(*color1_g) || check_color(*color1_b)) print_not_valid_args();
    if (*x_count > bmp_file->dhdr.width || *y_count > bmp_file->dhdr.height || *x_count < 1 || *y_count < 1) print_not_valid_args();
    if (*line_width > bmp_file->dhdr.width || *line_width > bmp_file->dhdr.height) print_not_valid_args();

    int dx = bmp_file->dhdr.width  / *x_count;
    int dy = bmp_file->dhdr.height / *y_count;

    unsigned char r = (unsigned char)*color1_r, g = (unsigned char)*color1_g, b = (unsigned char)*color1_b;

    for(int i = 0; i < *x_count - 1; i++) {
        bmp_file = draw_some_lines(bmp_file, dx * (i+1), 0, dx * (i+1), bmp_file->dhdr.height - 1, r, g, b, *line_width);
    }

    for(int i = 0; i < *y_count - 1; i++) {
        bmp_file = draw_some_lines(bmp_file, 0, dy * (i+1), bmp_file->dhdr.width - 1, dy * (i+1),  r, g, b, *line_width);
    }

    *x_count = -1, *y_count = -1, *line_width = -1, *color1_r = -1, *color1_g = -1, *color1_b = -1;

    return bmp_file;
}