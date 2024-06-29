#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "external_sort_utils.h"
#include "file_utils.h"
#include "random_numbers.h"
#include "sorting_algorithms.h"

#define MIN_MEMORY_SIZE (100 * 1024 * 1024)
#define MIN_ARG_COUNT_GENERATE 5
#define MIN_ARG_COUNT_SORT_INTERNAL 5
#define MIN_ARG_COUNT_SORT_EXTERNAL 6

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "1. Generate random numbers:\n");
    fprintf(stderr, "   %s generate <num_values> <max_value> <output_file>\n", program_name);
    fprintf(stderr, "   - <num_values>: Number of random values to generate.\n");
    fprintf(stderr, "   - <max_value>: Maximum value for the generated numbers.\n");
    fprintf(stderr, "   - <output_file>: File to save the generated random numbers.\n");
    fprintf(stderr, "   Example: %s generate 1000 100 output.txt\n\n", program_name);

    fprintf(stderr, "2. Sort numbers internally:\n");
    fprintf(stderr, "   %s sort_internal <input_file> <output_file> <algorithm>\n", program_name);
    fprintf(stderr, "   - <input_file>: File containing numbers to sort.\n");
    fprintf(stderr, "   - <output_file>: File to save the sorted numbers.\n");
    fprintf(stderr, "   - <algorithm>: Sorting algorithm to use (merge_shell, quick_insertion).\n");
    fprintf(stderr, "   Example: %s sort_internal input.txt output.txt merge_shell\n\n", program_name);

    fprintf(stderr, "3. Sort numbers externally:\n");
    fprintf(stderr, "   %s sort_external <input_file> <output_file> <algorithm> <memory_size>\n", program_name);
    fprintf(stderr, "   - <input_file>: File containing numbers to sort.\n");
    fprintf(stderr, "   - <output_file>: File to save the sorted numbers.\n");
    fprintf(stderr, "   - <algorithm>: Sorting algorithm to use (merge_shell, quick_insertion).\n");
    fprintf(stderr, "   - <memory_size>: Memory size to use for sorting, must be at least 100MB, e.g., 100M, 200M.\n");
    fprintf(stderr, "   Example: %s sort_external input.txt output.txt quick_insertion 150M\n\n", program_name);
}

char *strndup(const char *s, size_t n) {
    size_t len = strlen(s);
    if (n < len) {
        len = n;
    }

    char *new_str = (char *)malloc(len + 1);
    if (!new_str) {
        return NULL;
    }

    strncpy(new_str, s, len);
    new_str[len] = '\0';

    return new_str;
}

int parse_memory_size(const char *arg) {
    int len = strlen(arg);
    if (len == 0) {
        fprintf(stderr, "Invalid memory size\n");
        exit(EXIT_FAILURE);
    }

    int multiplier = 1;
    if (arg[len - 1] == 'M' || arg[len - 1] == 'm') {
        multiplier = 1024 * 1024;
        len--;
    }

    if (arg[len - 1] == 'G' || arg[len - 1] == 'g') {
        multiplier = 1024 * 1024 * 1024;
    }

    char *size_str = strndup(arg, len);
    int size = atoi(size_str);
    free(size_str);

    printf("Parsed memory size: %d bytes\n", size * multiplier);

    return size * multiplier;
}

SortAlgorithmConfig initialize_sort_algorithm_config(const char *algorithm) {
    SortAlgorithmConfig config;
    if (strcmp(algorithm, "merge_shell") == 0) {
        config.algorithm_name = "merge_shell";
        config.sort_func = merge_shell_sort;
        config.threshold = 10;
    } else if (strcmp(algorithm, "quick_insertion") == 0) {
        config.algorithm_name = "quick_insertion";
        config.sort_func = quick_insertion_sort;
        config.threshold = 10;
    } else {
        fprintf(stderr, "Unknown sorting algorithm: %s\n", algorithm);
        exit(EXIT_FAILURE);
    }
    return config;
}

void handle_generate_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_GENERATE) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    GenerateParams params;
    params.max_value = (size_t)atoi(argv[2]);
    params.num_values = atoi(argv[3]);
    params.output_file = argv[4];

    generate_and_save_uniform_random_numbers(&params);
    printf("Generated %ld random numbers up to %d and saved to %s.\n", params.num_values, params.max_value,
           params.output_file);
}

void handle_sort_internal_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_SORT_INTERNAL) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    SortInternalParams params;
    params.input_file = argv[2];
    params.output_file = argv[3];
    SortAlgorithmConfig algorithm_config = initialize_sort_algorithm_config(argv[4]);

    size_t count;
    int *numbers = read_numbers_from_file(params.input_file, &count);

    algorithm_config.sort_func(numbers, 0, count - 1, algorithm_config.threshold);

    FILE *fp = open_file_for_writing(params.output_file);
    int i;
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d\n", numbers[i]);
    }
    close_file(fp);

    free(numbers);
    printf("Sorted numbers saved to %s using %s algorithm.\n", params.output_file, params.algorithm);
}

void handle_sort_external_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_SORT_EXTERNAL) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    SortExternalParams params;
    params.input_file = argv[2];
    params.output_file = argv[3];
    SortAlgorithmConfig algorithm_config = initialize_sort_algorithm_config(argv[4]);

    int memory_size = parse_memory_size(argv[5]);
    if (memory_size < MIN_MEMORY_SIZE) {
        fprintf(stderr, "Memory size must be at least %d bytes.\n", MIN_MEMORY_SIZE);
        exit(EXIT_FAILURE);
    }
    params.num_elements = memory_size / sizeof(int);

    external_sort(&params, &algorithm_config);
    printf("Externally sorted numbers saved to %s using %s algorithm with memory size %d bytes.\n", params.output_file,
           algorithm_config.algorithm_name, memory_size);
}
