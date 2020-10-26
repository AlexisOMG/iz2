//
// Created by alexis on 10/25/20.
//

#include "sort.h"

void free_file_info(File_info *info) {
    if (info != NULL && info->path_to_file != NULL) {
        free(info->path_to_file);
        info->path_to_file = NULL;
        info->count_entry = -1;
    }
}

static void merge_arrays(File_info *arr, int left, int middle, int right, int (*compare)(File_info *a, File_info *b),
        void (*assign)(File_info *receiver, File_info *source)) {
    int left_block = 0;
    int right_block = 0;
    File_info *tmp = (File_info*)calloc(right - left, sizeof(File_info));
    while (left + left_block < middle && middle + right_block < right) {
        free_file_info(&tmp[left_block + right_block]);
        if (compare(&arr[left + left_block], &arr[middle + right_block]) < 0) {
            assign(&tmp[left_block + right_block], &arr[left + left_block]);
            ++left_block;
        } else {
            assign(&tmp[left_block + right_block], &arr[middle + right_block]);
            ++right_block;
        }
    }
    while (left + left_block < middle || middle + right_block < right) {
        if (left + left_block < middle) {
            free_file_info(&tmp[left_block + right_block]);
            assign(&tmp[left_block + right_block], &arr[left + left_block]);
            ++left_block;
        }
        if (middle + right_block < right) {
            free_file_info(&tmp[left_block + right_block]);
            assign(&tmp[left_block + right_block], &arr[middle + right_block]);
            ++right_block;
        }
    }
    for (int i = 0; i < left_block + right_block; i++) {
        free_file_info(&arr[left + i]);
        assign(&arr[left + i], &tmp[i]);
    }
    for (int i = 0; i < right - left; ++i) {
        free_file_info(&tmp[i]);
    }
    free(tmp);
}


void merge_sort(File_info *arr, int size, int (*compare)(File_info *a, File_info *b),
        void (*assign)(File_info *receiver, File_info *source)) {
    for (int block_size = 1; block_size < size; block_size *= 2) {
        for (int block = 0; block < size - block_size; block += 2 * block_size) {
            merge_arrays(arr, block, block + block_size,
                    block + 2 * block_size < size ? block + 2 * block_size : size, compare, assign);
        }
    }
}

