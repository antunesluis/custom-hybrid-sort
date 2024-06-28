#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "file_utils.h"
#include "random_numbers.h"
#include "sort.h"

#define MIN_MEMORY_SIZE (100 * 1024 * 1024)  // 100 MB
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
    fprintf(stderr, "   - <algorithm>: Sorting algorithm to use (merge_insertion, quick_insertion).\n");
    fprintf(stderr, "   Example: %s sort_internal input.txt output.txt merge_insertion\n\n", program_name);

    fprintf(stderr, "3. Sort numbers externally:\n");
    fprintf(stderr, "   %s sort_external <input_file> <output_file> <algorithm> <memory_size>\n", program_name);
    fprintf(stderr, "   - <input_file>: File containing numbers to sort.\n");
    fprintf(stderr, "   - <output_file>: File to save the sorted numbers.\n");
    fprintf(stderr,
            "   - <algorithm>: Sorting algorithm to use (external_merge_insertion, external_quick_insertion).\n");
    fprintf(stderr, "   - <memory_size>: Memory size to use for sorting, must be at least 100MB, e.g., 100M, 200M.\n");
    fprintf(stderr, "   Example: %s sort_external input.txt output.txt external_merge_insertion 150M\n\n",
            program_name);
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

    printf("Parsed memory size: %d bytes\n", size * multiplier);  // Debug statement

    return size * multiplier;
}

void handle_generate_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_GENERATE) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    size_t num_values = atoi(argv[2]);
    int max_value = atoi(argv[3]);
    const char *output_file = argv[4];

    generate_and_save_uniform_random_numbers(output_file, num_values, 0, max_value);
    printf("Generated %ld random numbers up to %d and saved to %s.\n", num_values, max_value, output_file);
}

void handle_sort_internal_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_SORT_INTERNAL) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[2];
    const char *output_file = argv[3];
    const char *algorithm = argv[4];

    size_t count;
    int *numbers = read_numbers_from_file(input_file, &count);

    if (strcmp(algorithm, "merge_insertion") == 0) {
        merge_insertion_sort(numbers, 0, count - 1);
    } else {
        fprintf(stderr, "Unknown sorting algorithm: %s\n", algorithm);
        free(numbers);
        exit(EXIT_FAILURE);
    }

    FILE *fp = open_file_for_writing(output_file);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\n", numbers[i]);
    }
    close_file(fp);

    free(numbers);
    printf("Sorted numbers saved to %s using %s algorithm.\n", output_file, algorithm);
}

void handle_sort_external_command(int argc, const char *argv[]) {
    if (argc != MIN_ARG_COUNT_SORT_EXTERNAL) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[2];
    const char *output_file = argv[3];
    const char *algorithm = argv[4];

    int memory_size = parse_memory_size(argv[5]);
    printf("Parsed memory size: %d bytes\n", memory_size);  // Debug statement
    int num_elements = memory_size / sizeof(int);

    printf("\nInt memory size int: %ld bytes", sizeof(int));      // Debug statement
    printf("\nInt memory size u_int: %ld bytes", sizeof(u_int));  // Debug statement
    printf("\nNum elements: %d", num_elements);                   // Debug statement

    if (memory_size < MIN_MEMORY_SIZE) {
        fprintf(stderr, "Memory size must be at least.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(algorithm, "merge_insertion") != 0) {
        fprintf(stderr, "Unknown sorting algorithm: %s\n", algorithm);
        exit(EXIT_FAILURE);
    }
    external_merge_insertion_sort(input_file, output_file, num_elements);

    printf("\nExternally sorted numbers saved to %s using %s algorithm with memory size %d bytes.\n", output_file,
           algorithm, memory_size);
}
