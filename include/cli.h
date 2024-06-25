#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

void print_usage(const char *program_name);
void handle_generate_command(int argc, const char *argv[]);
void handle_sort_internal_command(int argc, const char *argv[]);
void handle_sort_external_command(int argc, const char *argv[]);

#endif
