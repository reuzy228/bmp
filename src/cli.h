#pragma once
#include "structs.h"

void printHelp();
void parse(int **arr, int count, char *opt_arg);
Arguments* choice(Arguments *arguments, int opt);
void cli(int argc, char **argv);