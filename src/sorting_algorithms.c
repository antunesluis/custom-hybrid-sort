#include "sorting_algorithms.h"

#include <stdio.h>
#include <stdlib.h>

void insertion_sort(int arr[], int left, int right) {
    int i;
    for (i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1, j;

    for (j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void quick_insertion_sort(int arr[], int low, int high, int threshold) {
    while (low < high) {
        if (high - low < threshold) {
            insertion_sort(arr, low, high);
            break;
        } else {
            int pi = partition(arr, low, high);

            if (pi - low < high - pi) {
                quick_insertion_sort(arr, low, pi - 1, threshold);
                low = pi + 1;
            } else {
                quick_insertion_sort(arr, pi + 1, high, threshold);
                high = pi - 1;
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int i, j, k;

    int *left_arr = (int *)malloc(n1 * sizeof(int));
    int *right_arr = (int *)malloc(n2 * sizeof(int));

    if (left_arr == NULL || right_arr == NULL) {
        perror("Erro ao alocar memória para arrays temporários");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n1; i++) left_arr[i] = arr[left + i];
    for (j = 0; j < n2; j++) right_arr[j] = arr[mid + 1 + j];

    i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }

    free(left_arr);
    free(right_arr);
}

void shell_sort(int arr[], int n) {
    int gap, i, j;
    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            int temp = arr[i];
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void merge_shell_sort(int arr[], int left, int right, int threshold) {
    if (right - left + 1 <= threshold) {
        shell_sort(&arr[left], right - left + 1);
        return;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_shell_sort(arr, left, mid, threshold);
        merge_shell_sort(arr, mid + 1, right, threshold);
        merge(arr, left, mid, right);
    }
}
