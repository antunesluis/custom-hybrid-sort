#include "sort.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define TEMP_FILE_PREFIX "temp_sorted_"

// Função para mesclar dois subarrays usando Merge Sort
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Arrays temporários
    int *left_arr = (int *)malloc(n1 * sizeof(int));
    int *right_arr = (int *)malloc(n2 * sizeof(int));
    // Copiar dados para arrays temporários

    for (int i = 0; i < n1; i++) left_arr[i] = arr[left + i];
    for (int j = 0; j < n2; j++) right_arr[j] = arr[mid + 1 + j];

    // Mesclar os arrays temporários de volta para arr[left..right]
    int i = 0, j = 0, k = left;
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

    // Copiar elementos restantes de left_arr[], se houver
    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    // Copiar elementos restantes de right_arr[], se houver
    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }

    // Liberar memória dos arrays temporários
    free(left_arr);
    free(right_arr);
}

// Função Insertion Sort para pequenos trechos do array
void insertion_sort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Implementação do Merge Sort híbrido com Insertion Sort
void merge_insertion_sort(int arr[], int left, int right) {
    if (left < right) {
        // Se o tamanho do array é pequeno, usar Insertion Sort para otimização
        if (right - left + 1 <= 10) {
            insertion_sort(arr, left, right);
        } else {
            int mid = left + (right - left) / 2;
            merge_insertion_sort(arr, left, mid);
            merge_insertion_sort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
}

// Função para leitura dos números do arquivo de entrada
int read_block(FILE *file, int *buffer, int size) {
    int count = 0;
    while (count < size && fscanf(file, "%d", &buffer[count]) == 1) {
        count++;
    }
    return count;
}

// Função para gerar os subarquivos ordenados
void generate_sorted_runs(const char *input_file, int mem_size, int *num_runs) {
    FILE *input = fopen(input_file, "r");
    if (!input) {
        perror("Erro ao abrir o arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    int *buffer = (int *)malloc(mem_size * sizeof(int));
    *num_runs = 0;

    while (1) {
        int count = read_block(input, buffer, mem_size);
        if (count == 0) break;

        merge_insertion_sort(buffer, 0, count - 1);

        char temp_file_name[256];
        sprintf(temp_file_name, "%s%d.txt", TEMP_FILE_PREFIX, (*num_runs)++);
        FILE *temp_file = fopen(temp_file_name, "w");
        if (!temp_file) {
            perror("Erro ao abrir o arquivo temporário");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < count; i++) {
            fprintf(temp_file, "%d\n", buffer[i]);
        }

        fclose(temp_file);
    }

    free(buffer);
    fclose(input);
}

// Função para mesclar os subarquivos ordenados
void merge_sorted_runs(const char *output_file, int num_runs) {
    FILE *output = fopen(output_file, "w");
    if (!output) {
        perror("Erro ao abrir o arquivo de saída");
        exit(EXIT_FAILURE);
    }

    FILE **temp_files = (FILE **)malloc(num_runs * sizeof(FILE *));
    for (int i = 0; i < num_runs; i++) {
        char temp_file_name[256];
        sprintf(temp_file_name, "%s%d.txt", TEMP_FILE_PREFIX, i);
        temp_files[i] = fopen(temp_file_name, "r");
        if (!temp_files[i]) {
            perror("Erro ao abrir o arquivo temporário para leitura");
            exit(EXIT_FAILURE);
        }
    }

    int *current_values = (int *)malloc(num_runs * sizeof(int));
    int *active = (int *)malloc(num_runs * sizeof(int));

    for (int i = 0; i < num_runs; i++) {
        if (fscanf(temp_files[i], "%d", &current_values[i]) == 1) {
            active[i] = 1;
        } else {
            active[i] = 0;
        }
    }

    while (1) {
        int min_index = -1;
        int min_value = INT_MAX;
        for (int i = 0; i < num_runs; i++) {
            if (active[i] && current_values[i] < min_value) {
                min_value = current_values[i];
                min_index = i;
            }
        }

        if (min_index == -1) break;

        fprintf(output, "%d\n", min_value);

        if (fscanf(temp_files[min_index], "%d", &current_values[min_index]) != 1) {
            active[min_index] = 0;
        }
    }

    free(current_values);
    free(active);

    for (int i = 0; i < num_runs; i++) {
        fclose(temp_files[i]);
    }
    free(temp_files);
    fclose(output);
}

void external_merge_insertion_sort(const char *input_file, const char *output_file, int mem_size) {
    int num_runs;
    generate_sorted_runs(input_file, mem_size, &num_runs);
    merge_sorted_runs(output_file, num_runs);
}
