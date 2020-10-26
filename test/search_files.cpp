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
    ASSERT_EXIT(print_top_files(&path[0], &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Error with opening dir");
}

TEST(print_top_files, empty_input) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    ASSERT_EXIT(print_top_files(NULL, &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
    ASSERT_EXIT(print_top_files(&path[0], NULL), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
}

TEST(handle_dir, incorrect_path_to_dir) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    ASSERT_EXIT(handle_dir(&path[0], &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Error with opening dir");
}

TEST(handle_dir, empty_input) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    ASSERT_EXIT(handle_dir(NULL, &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
    ASSERT_EXIT(handle_dir(&path[0], NULL), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
}

TEST(print_top_files, correct_path_and_request) {
    testing::internal::CaptureStdout();
    std::string path = "../test_files\0";
    std::string req = "a\0";
    print_top_files(&path[0], &req[0]);
    std::string out = testing::internal::GetCapturedStdout();
    std::string test = "../test_files/8.txt 10\n../test_files/479.txt 10\n../test_files/182.txt 10\n../test_files/66.txt 10\n../test_files/183.txt 10\n";
    EXPECT_EQ(test, out);
}
