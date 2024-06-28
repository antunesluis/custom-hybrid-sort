#include <stdlib.h>
#include <string.h>

#include "cli.h"

#define MIN_ARGS 2

int main(int argc, char *argv[]) {
    if (argc < MIN_ARGS) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *command = argv[1];

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

    return EXIT_SUCCESS;
}
