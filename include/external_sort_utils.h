#ifndef EXTERNAL_SORT_UTILS_H
#define EXTERNAL_SORT_UTILS_H

typedef struct _sortExternalParams {
    const char *input_file;
    const char *output_file;
    const char *algorithm;
    int num_elements;
} SortExternalParams;

typedef struct _sortAlgorithmConfig {
    const char *algorithm_name;
    void (*sort_func)(int[], int, int, int);
    int threshold;
} SortAlgorithmConfig;

void external_sort(SortExternalParams *params, SortAlgorithmConfig *algorithm_config);

#endif
