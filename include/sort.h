#ifndef SORT_H
#define SORT_H

#include <sys/types.h>
void merge(int arr[], int left, int mid, int right);
void insertion_sort(int arr[], int left, int right);
void merge_insertion_sort(int arr[], int left, int right);
void external_merge_insertion_sort(const char *input_file, const char *output_file, int num_elements);

#endif
