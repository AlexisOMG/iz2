//
// Created by alexis on 10/26/20.
//

#include "search_files.h"

static void count_entry(char *path, char *request, char *path_to_count_file) {
    FILE *out = fopen(path_to_count_file, "a");
    if (out == NULL) {
        fprintf(stderr, "%s\n", "Unable to open file on writing");
        exit(EXIT_FAILURE);
    }
    fprintf(out, "%s ", path);
    if (fclose(out)) {
        fprintf(stderr, "%s\n", "Unable to close file");
        exit(EXIT_FAILURE);
    }
    char *grep = "grep -cF ";
    char *redirect = " >> ";
    char *cmd = (char*)calloc(strlen(grep) + strlen(request) + strlen(path) + strlen(redirect) + strlen(path_to_count_file) + 10,
            sizeof(char));
    cmd[0] = '\0';
    cmd = strncat(strncat(strncat(strncat(strncat(cmd, grep, strlen(grep)), request, strlen(request)), path, strlen(path)),
            redirect, strlen(redirect)), path_to_count_file, strlen(path_to_count_file));
    cmd = strncat(cmd, "\n", strlen("\n"));
    system(cmd);
    free(cmd);
}

static File_info* handle_files(int count_files, char *path_to_count_file) {
    if (count_files == 0) {
        return NULL;
    }
    FILE *in = fopen(path_to_count_file, "r");
    if (in == NULL) {
        fprintf(stderr, "%s\n", "Unable to open file");
        exit(EXIT_FAILURE);
    }

    File_info info;
    info.path_to_file = (char*)calloc(PATH_MAX_LENGTH, sizeof(char));

    File_info *arr = (File_info*)calloc(count_files, sizeof(File_info));
    int ind = 0;

    while (fscanf(in, "%127s%d", info.path_to_file, &info.count_entry) != EOF && ind < count_files) {
        assign(&arr[ind++], &info);
    }

    if (fclose(in)) {
        fprintf(stderr, "%s\n", "Unable to close file");
        exit(EXIT_FAILURE);
    }

    merge_sort(arr, count_files, compare, assign);
    free_file_info(&info);
    return arr;
}

int get_amount_of_files(char *path_to_dir) {
    if (path_to_dir == NULL || strlen(path_to_dir) == 0) {
        fprintf(stderr, "%s\n", "Incorrect path to dir");
        exit(EXIT_FAILURE);
    }
    DIR *dir = opendir(path_to_dir);
    if (dir == NULL) {
        fprintf(stderr, "%s\n", "Error with opening dir");
        exit(EXIT_FAILURE);
    }

    int cnt_files = 0;
    struct dirent *el;
    while ((el = readdir(dir)) != NULL) {
        if (el->d_type == 4 || strncmp(el->d_name, ".count", strlen(".count")) == 0) {
            continue;
        }
        ++cnt_files;
    }

    if (closedir(dir) == -1) {
        fprintf(stderr, "%s\n", "Error with closing dir");
        exit(EXIT_FAILURE);
    }

    return cnt_files;
}

File_info* handle_dir(char *path_to_dir, char *req) {
    if (path_to_dir == NULL || req == NULL || strlen(path_to_dir) == 0 || strlen(req) == 0) {
        fprintf(stderr, "%s\n", "Incorrect input");
        exit(EXIT_FAILURE);
    }
    char *request = (char*)malloc(strlen(req) + 4);
    request[0] = '\"';
    request[1] = '\0';
    request = strncat(strncat(strncat(request, req, strlen(req)), "\"", strlen("\"")), " ", strlen(" "));
    char *path_to_count_file = (char*)calloc(strlen(path_to_dir) + strlen(COUNT_FILE) + 1, sizeof(char));
    path_to_count_file = strncat(strncat(path_to_count_file, path_to_dir, strlen(path_to_dir)), COUNT_FILE, strlen(COUNT_FILE));

    DIR *dir = opendir(path_to_dir);
    if (dir == NULL) {
        free(path_to_count_file);
        free(request);
        fprintf(stderr, "%s\n", "Error with opening dir");
        exit(EXIT_FAILURE);
    }

    FILE *out = fopen(path_to_count_file, "w");
    if (out == NULL) {
        free(path_to_count_file);
        free(request);
        fprintf(stderr, "%s\n", "Unable to open file on writing");
        exit(EXIT_FAILURE);
    }
    if (fclose(out)) {
        free(path_to_count_file);
        free(request);
        fprintf(stderr, "%s\n", "Unable to close file");
        exit(EXIT_FAILURE);
    }

    int cnt_files = 0;
    struct dirent *el;
    while ((el = readdir(dir)) != NULL) {
        if (el->d_type == 4 || strncmp(el->d_name, ".count", strlen(".count")) == 0) {
            continue;
        }
        ++cnt_files;
        char *path = (char*)malloc(strlen(path_to_dir) + strlen(el->d_name) + 4);
        path[0] = '\0';
        path = strncat(strncat(strncat(path, path_to_dir, strlen(path_to_dir)), "/", strlen("/")),
                el->d_name, strlen(el->d_name));
        count_entry(path, request, path_to_count_file);
        free(path);
    }

    if (closedir(dir) == -1) {
        free(path_to_count_file);
        free(request);
        fprintf(stderr, "%s\n", "Error with closing dir");
        exit(EXIT_FAILURE);
    }


    File_info *result = handle_files(cnt_files, path_to_count_file);
    free(request);
    if (remove(path_to_count_file) == -1) {
        free(path_to_count_file);
        fprintf(stderr, "%s\n", "Unable to delete .count file");
        exit(EXIT_FAILURE);
    }
    free(path_to_count_file);
    return result;
}

void print_top_files(char *path_to_dir, char *request) {
    if (path_to_dir == NULL || request == NULL || strlen(path_to_dir) == 0 || strlen(request) == 0) {
        fprintf(stderr, "%s\n", "Incorrect input");
        exit(EXIT_FAILURE);
    }
    File_info *arr = handle_dir(path_to_dir, request);
    if (arr == NULL) {
        printf("No files\n");
        return;
    }
    int size = get_amount_of_files(path_to_dir);
    for (int i = 0; i < size && i < AMOUNT_TOP_FILES; ++i) {
        printf("%s %d\n", arr[i].path_to_file, arr[i].count_entry);
    }
    free(arr);
}

void assign(File_info *receiver, File_info *source) {
    if (receiver == NULL || source == NULL) {
        fprintf(stderr, "%s\n", "Unable to assign");
        exit(EXIT_FAILURE);
    }
    receiver->count_entry = source->count_entry;
    if (receiver->path_to_file != NULL) {
        free(receiver->path_to_file);
    }
    receiver->path_to_file = (char*)calloc(strlen(source->path_to_file) + 1, sizeof(char));
    receiver->path_to_file = strncpy(receiver->path_to_file, source->path_to_file, strlen(source->path_to_file) * sizeof(char));
    receiver->path_to_file[strlen(source->path_to_file)] = '\0';
}

int compare(File_info *a, File_info *b) {
    return b->count_entry - a->count_entry;
}
