//
// Created by alexis on 10/25/20.
//

#include <stdlib.h>

typedef struct file_info {
    char *path_to_file;
    int count_entry;
} File_info;

void free_file_info(File_info *info);

void merge_sort(File_info *arr, int size, int (*compare)(File_info *a, File_info *b),
        void (*assign)(File_info *receiver, File_info *source));
