//
// Created by alexis on 10/26/20.
//

#include "gtest/gtest.h"
#include <string>

extern "C" {
#include "search_files.h"
}

TEST(print_top_files, incorrect_path_to_dir) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    ASSERT_EXIT(print_top_files(&path[0], &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "");
}

//TEST(print_top_files, correct_path_and_request) {
//
//}
