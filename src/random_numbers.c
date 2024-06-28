#include "random_numbers.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int uniform_distribution(int range_low, int range_high) {
    int range = range_high - range_low + 1;
    int copies = RAND_MAX / range;
    int limit = range * copies;

    int myRand;
    do {
        myRand = rand();
    } while (myRand >= limit);

    return myRand / copies + range_low;
}

void generate_and_save_uniform_random_numbers(GenerateParams *params) {
    srand(time(NULL));

    FILE *fp = fopen(params->output_file, "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < params->num_values; i++) {
        int number = uniform_distribution(0, params->max_value);
        fprintf(fp, "%d\n", number);
    }

    fclose(fp);
    printf("Números aleatórios inteiros uniformemente distribuídos foram salvos em '%s'.\n", params->output_file);
}
