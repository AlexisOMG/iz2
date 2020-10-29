//
// Created by alexis on 10/24/20.
//

#include "search_files.h"

//argv[1] - path to directory with files
//argv[2] - request
int main(int argc, char *argv[]) {
    print_top_files(argv[1], argv[2]);
    return EXIT_SUCCESS;
}
