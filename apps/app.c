#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cli.h"

#define MIN_ARGS 2

int main(int argc, char *argv[]) {
    if (argc < MIN_ARGS) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *command = argv[1];

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    if (strcmp(command, "generate") == 0) {
        handle_generate_command(argc, (const char **)argv);
    } else if (strcmp(command, "sort_internal") == 0) {
        handle_sort_internal_command(argc, (const char **)argv);
    } else if (strcmp(command, "sort_external") == 0) {
        handle_sort_external_command(argc, (const char **)argv);
    } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sort time: %f seconds\n", cpu_time_used);

    return EXIT_SUCCESS;
}
