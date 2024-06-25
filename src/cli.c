#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "random_numbers.h"
#include "sort.h"
void print_usage(const char *program_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "1. Generate random numbers:\n");
    fprintf(stderr, "   %s generate <num_values> <max_value> <output_file>\n", program_name);
    fprintf(stderr, "   - <num_values>: Number of random values to generate.\n");
    fprintf(stderr, "   - <max_value>: Maximum value for the generated numbers.\n");
    fprintf(stderr, "   - <output_file>: File to save the generated random numbers.\n\n");

    fprintf(stderr, "2. Sort numbers internally:\n");
    fprintf(stderr, "   %s sort_internal <input_file> <output_file> <algorithm>\n", program_name);
    fprintf(stderr, "   - <input_file>: File containing numbers to sort.\n");
    fprintf(stderr, "   - <output_file>: File to save the sorted numbers.\n");
    fprintf(stderr, "   - <algorithm>: Sorting algorithm to use (merge_insertion, quick_insertion).\n\n");

    fprintf(stderr, "3. Sort numbers externally:\n");
    fprintf(stderr, "   %s sort_external <input_file> <output_file> <algorithm> <memory_size>\n", program_name);
    fprintf(stderr, "   - <input_file>: File containing numbers to sort.\n");
    fprintf(stderr, "   - <output_file>: File to save the sorted numbers.\n");
    fprintf(stderr,
            "   - <algorithm>: Sorting algorithm to use (external_merge_insertion, external_quick_insertion).\n");
    fprintf(stderr, "   - <memory_size>: Memory size to use for sorting, e.g., 100K, 200M.\n");
}

int parse_memory_size(const char *arg) {
    int len = strlen(arg);
    if (len == 0) {
        fprintf(stderr, "Invalid memory size\n");
        exit(EXIT_FAILURE);
    }

    int multiplier = 1;
    if (arg[len - 1] == 'K' || arg[len - 1] == 'k') {
        multiplier = 1024;
        len--;
    } else if (arg[len - 1] == 'M' || arg[len - 1] == 'm') {
        multiplier = 1024 * 1024;
        len--;
    } else if (arg[len - 1] == 'G' || arg[len - 1] == 'g') {
        multiplier = 1024 * 1024 * 1024;
    }

    char *size_str = strndup(arg, len);
    int size = atoi(size_str);
    free(size_str);

    return size * multiplier;
}

void handle_generate_command(int argc, const char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    size_t num_values = atoi(argv[2]);
    int max_value = atoi(argv[3]);
    const char *output_file = argv[4];

    generate_and_save_uniform_random_numbers(output_file, num_values, 0, max_value);
    printf("Generated %lld random numbers up to %d and saved to %s.\n", num_values, max_value, output_file);
}

void handle_sort_internal_command(int argc, const char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[2];
    const char *output_file = argv[3];
    const char *algorithm = argv[4];

    int count;
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
    if (argc != 6) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[2];
    const char *output_file = argv[3];
    const char *algorithm = argv[4];
    int memory_size = parse_memory_size(argv[5]);

    if (strcmp(algorithm, "merge_insertion") == 0) {
        external_merge_insertion_sort(input_file, output_file, memory_size);
    } else {
        fprintf(stderr, "Unknown sorting algorithm: %s\n", algorithm);
        exit(EXIT_FAILURE);
    }

    printf("Externally sorted numbers saved to %s using %s algorithm with memory size %d bytes.\n", output_file,
           algorithm, memory_size);
}
