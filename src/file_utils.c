#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>

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

    for (int i = 0; i < count; i++) {
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

int *read_numbers_from_file(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int capacity = 10;
    int *numbers = malloc(capacity * sizeof(int));
    *count = 0;

    while (fscanf(file, "%d", &numbers[*count]) == 1) {
        (*count)++;
        if (*count >= capacity) {
            capacity *= 2;
            numbers = realloc(numbers, capacity * sizeof(int));
        }
    }

    fclose(file);
    return numbers;
}
