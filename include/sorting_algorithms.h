#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

void insertion_sort(int arr[], int left, int right);
void quick_insertion_sort(int arr[], int low, int high, int threshold);
void merge(int arr[], int left, int mid, int right);
void shell_sort(int arr[], int n);
void merge_shell_sort(int arr[], int left, int right, int threshold);

#endif
