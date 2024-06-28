#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include <stdio.h>

void process_file(const char *input_file, const char *output_file, const char *algorithm);
bool validate_input_files(const char *input_file, const char *output_file);
int *read_and_sort_numbers(const char *input_file, int *count, const char *algorithm);
bool write_numbers_to_file(const char *output_file, const int *numbers, int count);
bool is_file(const char *arg);
FILE *open_file_for_writing(const char *filename);
void close_file(FILE *fp);
int *read_numbers_from_file(const char *filename, size_t *count);

#endif
