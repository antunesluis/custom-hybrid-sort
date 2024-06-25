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

void generate_and_save_uniform_random_numbers(const char *filename, int num_numbers, int min, int max) {
    srand(time(NULL));

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_numbers; i++) {
        int number = uniform_distribution(min, max);
        fprintf(fp, "%d\n", number);
    }

    fclose(fp);
    printf("Números aleatórios inteiros uniformemente distribuídos foram salvos em '%s'.\n", filename);
}
