//
// Created by alexis on 10/26/20.
//

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "sort.h"

#define COUNT_FILE "/.count"
#define PATH_MAX_LENGTH 127

void print_top_files(char *path_to_dir, char *request);

File_info* handle_dir(char *path_to_dir, char *req);

void assign(File_info *receiver, File_info *source);

int compare(File_info *a, File_info *b);
