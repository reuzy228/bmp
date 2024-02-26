#include "headers.h"
#include "functions.h"
#include "structs.h"
#include "bmpWork.h"
#pragma pack (push,1)

const struct option long_options[] = {
    {"mirror",       no_argument,       NULL, 'm'},
    {"copy",         no_argument,       NULL, 'c'},
    {"replace",      no_argument,       NULL, 'r'},
    {"divide",       no_argument,       NULL, 'd'},
    {"first",        required_argument, NULL, 'f'},
    {"second",       required_argument, NULL, 's'},
    {"third",        required_argument, NULL, 't'},
    {"axis",         required_argument, NULL, 'a'},
    {"xCount",       required_argument, NULL, 'x'},
    {"yCount",       required_argument, NULL, 'y'},
    {"width",        required_argument, NULL, 'w'},
    {"FirstColor",   required_argument, NULL, 'F'},
    {"SecondColor",  required_argument, NULL, 'S'},
    {"Color",        required_argument, NULL, 'C'},
    {"output",       required_argument, NULL, 'o'},
    {"help",         no_argument,       NULL, 'h'},
    {"info",         no_argument,       NULL, 'i'},
    {NULL,           no_argument,       NULL,  0 } 
};

#pragma pack(pop)

void printHelp() {
    printf("Руководство по использованию программы:\n");
    printf("Программа обрабатывает BMP-файлы версии 3. С глубиной изображения 24 бита на пиксель\n");
    printf("Формат ввода: [имя исполняемого файла] [имя BMP-файла для обработки] [функция] -[ключ]/--[полный ключ] [аргумент] ... \n");
    printf("Функции/ключи:\n");

    printf("-m/--mirror -- отражение заданной области.\n");
    printf("\t-a/--axis   [<число>] - выбор оси относительно которой отражать (0 - вертикальная, 1 - горизонтальная)\n");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата левого верхнего угла области\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата правого нижнего угла области\n");

    printf("-c/--copy -- копирование заданной области\n");
    printf("\t-f/--first  [<x-координата>.<y-координата>] - координата левого верхнего угла области-источника\n");
    printf("\t-s/--second [<x-координата>.<y-координата>] - координата правого нижнего угла области-источника\n");
    printf("\t-t/--third  [<x-координата>.<y-координата>] - координата левого верхнего угла области-назначения\n");

    printf("-r/--replace -- замена всех пикселей одного заданного цвета на другой цвет\n");
    printf("\t-F/--FirstColor  [<число>.<число>.<число>] - цвет который требуется заменить (RGB)\n");
    printf("\t-S/--SecondColor [<число>.<число>.<число>] - цвет на который требуется заменить (RGB)\n");

    printf("-d/--divide -- разделить изображение на N*M частей\n");
    printf("\t-y/--yCount [<число>] - количество частей по оси \"Y\"\n");
    printf("\t-x/--xCount [<число>] - количество частей по оси \"X\"\n");
    printf("\t-w/--width  [<число>] - толщина линии\n");
    printf("\t-C/--Color  [<число>.<число>.<число>] - цвет линии (RGB)\n");

    printf("-h/--help - вывод справки о работе программы\n");
    printf("-i/--info - вывод подробной информации о bmp-файле\n");
    printf("-o/--output [путь] - файл для вывода (по умолчанию исходный файл, путь - относительный)\n");
}

void parse(int **arr, int count, char *opt_arg) {
    char *str;
    str = strtok(opt_arg, ".");
    *(arr[0]) = atoi(str);
    for(int i = 1; i < count; i++) {
        str = strtok(NULL, ".");
        if(str != NULL) *(arr[i]) = atoi(str);
        else {
            printf("Было введено недостаточно чисел в одном аргументе. Работа программы завершена.\n");
            exit(-1);
        }
    }
}


Arguments* choice(Arguments *arguments, int opt) {
    int **arr = malloc(3 * sizeof(int*));

    switch (opt) {
        case 'f':
            arr[0] = &(arguments->x1), arr[1] = &(arguments->y1);
            parse(arr, 2, optarg);
            break;
        case 's':
            arr[0] = &(arguments->x2), arr[1] = &(arguments->y2);
            parse(arr, 2, optarg);
            break;
        case 't':
            arr[0] = &(arguments->x3), arr[1] = &(arguments->y3);
            parse(arr, 2, optarg);
            break;
        case 'a':
            arguments->axis = atoi(optarg);
            break;
        case 'x':
            arguments->x_count = atoi(optarg);
            break;
        case 'y':
            arguments->y_count = atoi(optarg);
            break;
        case 'w':
            arguments->line_width = atoi(optarg);
            break;
        case 'F':
            arr[0] = &(arguments->color1_r), arr[1] = &(arguments->color1_g), arr[2] = &(arguments->color1_b);
            parse(arr, 3, optarg);
            break;
        case 'S':
            arr[0] = &(arguments->color2_r), arr[1] = &(arguments->color2_g), arr[2] = &(arguments->color2_b);
            parse(arr, 3, optarg);
            break;
        case 'C':
            arr[0] = &(arguments->color1_r), arr[1] = &(arguments->color1_g), arr[2] = &(arguments->color1_b);
            parse(arr, 3, optarg);
            break;
        case 'o':
            arguments->output = malloc(strlen(optarg)+1);
            strcpy(arguments->output, optarg);
            break;
        case 'h':
            printHelp();
            break;
    };

    return arguments;
}

BMPFile* functions_choice(int opt, int prev_opt, Arguments *arguments, BMPFile* bmp_file) {
    if (opt == 'm' || opt == 'c' || opt == 'r' || opt == 'd') {
        switch (prev_opt) {
            case 'm':
                bmp_file = mirror(bmp_file, arguments);
                break;
            case 'c':
                bmp_file = copy(bmp_file, arguments);
                break;
            case 'r':
                bmp_file = replace(bmp_file, arguments);
                break;
            case 'd':
                bmp_file = divide(bmp_file, arguments);
                break;     
        }
    }
    return bmp_file;
}

Arguments* arg_set_to_NULL(Arguments* arguments) {
    arguments->x1 = -1, arguments->x2 = -1, arguments->x3 = -1;
    arguments->y1 = -1, arguments->y2 = -1, arguments->y3 = -1;
    arguments->axis = -1, arguments->line_width = -1, arguments->output = NULL;
    arguments->color1_r = -1, arguments->color1_g = -1, arguments->color1_b = -1;
    arguments->color2_r = -1, arguments->color2_g = -1, arguments->color2_b = -1;
    arguments->x_count = -1, arguments->y_count = -1;
    return arguments;
}


void cli(int argc, char **argv) {
    char *opts = "mcrdf:s:t:a:x:y:w:F:S:C:o:hi";
    Arguments *arguments = malloc(sizeof(Arguments));
    arguments = arg_set_to_NULL(arguments);

    
    int opt;
    int prev_opt = 'N';
    int long_index;
    char* file_name = argv[1];

    if (argc == 1) {
        printHelp();
        exit(-1);
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printHelp();
        exit(-1);
    }

    BMPFile* bmp_file = loadBMPFile(file_name);

    arguments->output = file_name;

    opt = getopt_long(argc, argv, opts, long_options, &long_index);
    while (opt != -1) {

        bmp_file = functions_choice(opt, prev_opt, arguments, bmp_file);

        if (opt == 'm' || opt == 'c' || opt == 'r' || opt == 'd') prev_opt = opt;
        if (opt == 'i') {
            print_bmp_info(bmp_file);
        }

        arguments = choice(arguments, opt);

        opt = getopt_long(argc, argv, opts, long_options, &long_index);

    }

    bmp_file = functions_choice('m', prev_opt, arguments, bmp_file);    

    output_bmp_file(bmp_file, arguments->output);
}