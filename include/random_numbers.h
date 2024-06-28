#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include <stdio.h>

typedef struct _generateParams {
    size_t num_values;
    int max_value;
    const char *output_file;
} GenerateParams;

void generate_and_save_uniform_random_numbers(GenerateParams *params);

#endif
