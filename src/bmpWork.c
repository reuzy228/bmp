#include "headers.h"
#include "bmpWork.h"
#include "structs.h"

void readBMP(FILE* fp, BMPFile** bmp_file) {
    int bytes_per_pixel = (*bmp_file)->dhdr.bits_per_pixel / 8 ;
    int row_size = bytes_per_pixel * (*bmp_file)->dhdr.width;
    int row_padding = (4 - (row_size % 4)) % 4;
    int rows_written = 0;
    int cur_col;
    int cur_row = 0;
    int height = (*bmp_file)->dhdr.height - 1;
    unsigned char* row = malloc(row_size + row_padding);
    fseek(fp, (*bmp_file)->bhdr.pixel_offset, SEEK_SET);

    for(int cur_row = 0; cur_row < height + 1; cur_row++) {
        cur_col = 0;
        fread(row, row_size+row_padding, 1 , fp);

        for(int i = 0; i < row_size; i += bytes_per_pixel) {
            (*bmp_file)->pixel_data[height - cur_row][cur_col].blue = row[i];
            (*bmp_file)->pixel_data[height - cur_row][cur_col].green = row[i+1];
            (*bmp_file)->pixel_data[height - cur_row][cur_col].red = row[i+2];
            cur_col++;
        }
    }
    free(row);
}

BMPFile* loadBMPFile(char* file_name) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("Проблемы с открытием bmp-файла. Работа программы завершена\n");
        exit(-1);
    }

    BMPFile* bmp_file = malloc(sizeof(BMPFile));
    fread(&bmp_file->bhdr, sizeof(BMPHeader), 1, fp);
    fread(&bmp_file->dhdr, sizeof(DIBHeader), 1, fp);

    if (bmp_file->dhdr.bits_per_pixel != 24) {
        printf("Программа работает только с файлами у которых выделено 24 бита на цвет. Работа программы завершена\n");
        exit(-1);
    }

    bmp_file->pixel_data = malloc(bmp_file->dhdr.height * sizeof(Pixel_data*));
    for(int i = 0; i < bmp_file->dhdr.height; i++) {
        bmp_file->pixel_data[i] = malloc(bmp_file->dhdr.width * sizeof(Pixel_data));
    }
    
    readBMP(fp, &bmp_file);


    fclose(fp);
    return bmp_file;

}   

void freeBMPFile(BMPFile* bmp_file) {
    if (bmp_file) {
        for(int i = 0; i < bmp_file->dhdr.height; i++) {
            free(bmp_file->pixel_data[i]);
        }
        free(bmp_file->pixel_data);
        free(bmp_file);
    }
}

void print_bmp_info(BMPFile* bmp_file) {
    printf("Информация о bmp-файле:\n");
    printf("ID = \t\t\t%c%c\n",bmp_file->bhdr.ID[0], bmp_file->bhdr.ID[1]);
    printf("file_size = \t\t%d\n",bmp_file->bhdr.file_size);
    printf("pixel_offset = \t\t%d\n",bmp_file->bhdr.pixel_offset);
    printf("header_size = \t\t%d\n",bmp_file->dhdr.header_size);
    printf("width = \t\t%d\n",bmp_file->dhdr.width);
    printf("height = \t\t%d\n",bmp_file->dhdr.height);
    printf("color_planes = \t\t%d\n",bmp_file->dhdr.color_planes);
    printf("bits_per_pixel = \t%d\n",bmp_file->dhdr.bits_per_pixel);
    printf("compression = \t\t%d\n",bmp_file->dhdr.compression);
    printf("data_size = \t\t%d\n",bmp_file->dhdr.data_size);
    printf("pwidth = \t\t%d\n",bmp_file->dhdr.pwidth);
    printf("pheight = \t\t%d\n",bmp_file->dhdr.pheight);
    printf("colors_count = \t\t%d\n",bmp_file->dhdr.colors_count);
    printf("imp_colors_count = \t%d\n",bmp_file->dhdr.imp_colors_count);
}

void print_bmp_pixels(BMPFile* bmp_file) {
    for(int i = 0 ; i < bmp_file->dhdr.height; i++) {
        for(int j = 0; j < bmp_file->dhdr.width; j++) {
            printf("%02x %02x %02x ", bmp_file->pixel_data[i][j].red, bmp_file->pixel_data[i][j].green, bmp_file->pixel_data[i][j].blue);
        }
        printf("\n");
    }
}

unsigned char* rgb_to_str(BMPFile* bmp_file) {
    int bytes_per_pixel = bmp_file->dhdr.bits_per_pixel / 8 ;
    int row_size = bytes_per_pixel * bmp_file->dhdr.width;
    int row_padding = (4 - (row_size % 4)) % 4;
    int height = bmp_file->dhdr.height;
    int width = bmp_file->dhdr.width;

    unsigned char *data = malloc((row_size + row_padding) * bmp_file->dhdr.height);
    int index = 0;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            data[index++] = bmp_file->pixel_data[height - i - 1][j].blue;
            data[index++] = bmp_file->pixel_data[height - i - 1][j].green;
            data[index++] = bmp_file->pixel_data[height - i - 1][j].red;
        }
        for(int j = 0; j < row_padding; j++) {
            data[index++] = (unsigned char)0;
        }
    }

    return data;

}

void output_bmp_file(BMPFile* bmp_file, char* output_file) {
    FILE *fp = fopen(output_file, "wb");

    if (!fp) {
        printf("Проблемы с созданием файла для вывода. Работа программы завершена\n");
        exit(-1);
    }

    fwrite(&bmp_file->bhdr, 1, sizeof(BMPHeader),fp);
    fwrite(&bmp_file->dhdr, 1, sizeof(DIBHeader),fp);

    unsigned char* data = rgb_to_str(bmp_file);

    int bytes_per_pixel = bmp_file->dhdr.bits_per_pixel / 8 ;
    int row_size = bytes_per_pixel * bmp_file->dhdr.width;
    int row_padding = (4 - (row_size % 4)) % 4;

    fwrite(data, 1, (row_size + row_padding) * bmp_file->dhdr.height, fp);

    free(data);
    fclose(fp);
}