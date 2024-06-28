#include "external_sort_utils.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define TEMP_FILE_PREFIX "temp_sorted_"

int read_block(FILE *file, int *buffer, int size) {
    int i;
    for (i = 0; i < size && fscanf(file, "%d", &buffer[i]) == 1; i++);
    return i;
}

void generate_sorted_runs(const char *input_file, int num_elements, int *num_runs, int threshold,
                          void (*sort_func)(int[], int, int, int)) {
    FILE *input = fopen(input_file, "r");
    if (!input) {
        perror("Erro ao abrir o arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    int *buffer = (int *)malloc(num_elements * sizeof(int));
    if (buffer == NULL) {
        perror("Erro ao alocar memória para buffer");
        fclose(input);
        exit(EXIT_FAILURE);
    }

    *num_runs = 0;

    while (1) {
        int count = read_block(input, buffer, num_elements);
        if (count == 0) break;

        sort_func(buffer, 0, count - 1, threshold);

        char temp_file_name[256];
        sprintf(temp_file_name, "%s%d.txt", TEMP_FILE_PREFIX, (*num_runs)++);
        FILE *temp_file = fopen(temp_file_name, "w");

        if (!temp_file) {
            perror("Erro ao abrir o arquivo temporário");
            free(buffer);
            fclose(input);
            exit(EXIT_FAILURE);
        }

        // Gravar os números ordenados no arquivo temporário
        for (int i = 0; i < count; i++) {
            fprintf(temp_file, "%d\n", buffer[i]);
        }

        fclose(temp_file);
    }

    free(buffer);
    fclose(input);
}

FILE **open_temp_files(int num_runs) {
    FILE **temp_files = (FILE **)malloc(num_runs * sizeof(FILE *));
    if (!temp_files) {
        perror("Error allocating memory for file pointers");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_runs; i++) {
        char temp_file[256];
        snprintf(temp_file, sizeof(temp_file), "%s%d.txt", TEMP_FILE_PREFIX, i);
        temp_files[i] = fopen(temp_file, "r");
        if (!temp_files[i]) {
            perror("Error opening temporary file");
            for (int j = 0; j < i; j++) {
                fclose(temp_files[j]);
            }
            free(temp_files);
            exit(EXIT_FAILURE);
        }
    }

    return temp_files;
}

void initialize_current_values(FILE **temp_files, int *current_values, int *active, int num_runs) {
    for (int i = 0; i < num_runs; i++) {
        if (fscanf(temp_files[i], "%d", &current_values[i]) == 1) {
            active[i] = 1;
        } else {
            active[i] = 0;
        }
    }
}

void merge_files(FILE *output, FILE **temp_files, int *current_values, int *active, int num_runs) {
    while (1) {
        int min_value = INT_MAX;
        int min_index = -1;

        for (int i = 0; i < num_runs; i++) {
            if (active[i] && current_values[i] < min_value) {
                min_value = current_values[i];
                min_index = i;
            }
        }

        if (min_index == -1) {
            break;
        }

        fprintf(output, "%d\n", min_value);

        if (fscanf(temp_files[min_index], "%d", &current_values[min_index]) != 1) {
            active[min_index] = 0;
        }
    }
}

void merge_sorted_runs(const char *output_file, int num_runs) {
    FILE *output = fopen(output_file, "w");
    if (!output) {
        perror("Erro ao abrir o arquivo de saída");
        exit(EXIT_FAILURE);
    }

    FILE **temp_files = open_temp_files(num_runs);

    int *current_values = (int *)malloc(num_runs * sizeof(int));
    int *active = (int *)malloc(num_runs * sizeof(int));
    if (current_values == NULL || active == NULL) {
        perror("Erro ao alocar memória para valores atuais ou ativos");
        for (int i = 0; i < num_runs; i++) {
            fclose(temp_files[i]);
        }
        free(temp_files);
        exit(EXIT_FAILURE);
    }

    initialize_current_values(temp_files, current_values, active, num_runs);
    merge_files(output, temp_files, current_values, active, num_runs);

    free(current_values);
    free(active);

    for (int i = 0; i < num_runs; i++) {
        fclose(temp_files[i]);
    }
    free(temp_files);
    fclose(output);
}

void external_sort(SortExternalParams *params, SortAlgorithmConfig *algorithm_config) {
    int num_runs;

    printf("\nComeçando a criar os temp!");
    generate_sorted_runs(params->input_file, params->num_elements, &num_runs, algorithm_config->threshold,
                         algorithm_config->sort_func);

    printf("\nComeçando a juntar os temp!");
    merge_sorted_runs(params->output_file, num_runs);
}
