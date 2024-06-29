#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>

#define CAPACITY_STARTER_VAlUE 10

bool validate_input_files(const char *input_file, const char *output_file) {
    if (!is_file(input_file)) {
        fprintf(stderr, "Erro: Arquivo '%s' não encontrado.\n", input_file);
        return false;
    }

    FILE *file = fopen(output_file, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return false;
    }
    fclose(file);
    return true;
}

bool write_numbers_to_file(const char *output_file, const int *numbers, int count) {
    FILE *fp = open_file_for_writing(output_file);
    if (fp == NULL) {
        return false;
    }

    int i;
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d\n", numbers[i]);
    }
    close_file(fp);
    return true;
}

bool is_file(const char *arg) {
    if (arg == NULL || *arg == '\0') {
        return false;
    }

    FILE *file = fopen(arg, "r");
    if (file == NULL) {
        return false;
    }

    fclose(file);
    return true;
}

FILE *open_file_for_writing(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void close_file(FILE *fp) { fclose(fp); }

int *read_numbers_from_file(const char *filename, size_t *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    size_t capacity = CAPACITY_STARTER_VAlUE;
    int *numbers = malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    *count = 0;

    while (fscanf(file, "%d", &numbers[*count]) == 1) {
        (*count)++;
        if (*count >= capacity) {
            capacity *= 2;
            int *temp = realloc(numbers, capacity * sizeof(int));
            if (temp == NULL) {
                perror("Erro ao realocar memória");
                free(numbers);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            numbers = temp;
        }
    }

    fclose(file);
    return numbers;
}
