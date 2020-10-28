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
    EXPECT_EXIT(print_top_files(&path[0], &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Error with opening dir");
}

TEST(print_top_files, empty_input) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    EXPECT_EXIT(print_top_files(NULL, &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
    EXPECT_EXIT(print_top_files(&path[0], NULL), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
}

TEST(handle_dir, incorrect_path_to_dir) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    EXPECT_EXIT(handle_dir(&path[0], &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Error with opening dir");
}

TEST(handle_dir, empty_input) {
    std::string path = "../../templates/test_files\0";
    std::string req = "a\0";
    EXPECT_EXIT(handle_dir(NULL, &req[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
    EXPECT_EXIT(handle_dir(&path[0], NULL), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect input");
}

TEST(print_top_files, no_files) {
    testing::internal::CaptureStdout();
    std::string path = "../../empty_dir\0";
    std::string req = "a\0";
    print_top_files(&path[0], &req[0]);
    std::string out = testing::internal::GetCapturedStdout();
    std::string test = "No files\n";
    EXPECT_EQ(out, test);
}

TEST(print_top_files, correct_path_and_request) {
    testing::internal::CaptureStdout();
    std::string path = "../../test_files\0";
    std::string req = "a\0";
    print_top_files(&path[0], &req[0]);
    std::string out = testing::internal::GetCapturedStdout();
    std::string test = "../../test_files/8.txt 10\n../../test_files/479.txt 10\n../../test_files/182.txt 10\n../../test_files/66.txt 10\n../../test_files/183.txt 10\n";
    EXPECT_EQ(out, test);
}

TEST(assign, null_pointers) {
    File_info *a, *b;
    std::string path = "ggwp";
    a = (File_info*)calloc(1, sizeof(File_info));
    a->path_to_file = &path[0];
    a->count_entry = 2;
    b = NULL;
    EXPECT_EXIT(assign(a, b), ::testing::ExitedWithCode(EXIT_FAILURE), "Unable to assign");
    free(a);
    a = NULL;
    b = (File_info*)calloc(1, sizeof(File_info));
    b->path_to_file = &path[0];
    b->count_entry = 2;
    EXPECT_EXIT(assign(a, b), ::testing::ExitedWithCode(EXIT_FAILURE), "Unable to assign");
    free(b);
}

TEST(assign, free_receiver) {
    File_info *a, *b;
    a = (File_info*)calloc(1, sizeof(File_info));
    a->path_to_file = (char*)calloc(2, sizeof(char));
    a->path_to_file[0] = 'a';
    a->path_to_file[1] = '\0';
    a->count_entry = 2;
    b = (File_info*)calloc(1, sizeof(File_info));
    b->path_to_file = (char*)calloc(2, sizeof(char));
    b->path_to_file[0] = 'b';
    b->path_to_file[1] = '\0';
    b->count_entry = 3;
    EXPECT_NO_FATAL_FAILURE(assign(a, b));
    EXPECT_EQ(a->count_entry, b->count_entry);
    EXPECT_STREQ(a->path_to_file, b->path_to_file);
    free(a);
    free(b);
}

TEST(get_amount_of_files, empty_path) {
    EXPECT_EXIT(get_amount_of_files(NULL), ::testing::ExitedWithCode(EXIT_FAILURE), "Incorrect path to dir");
}

TEST(get_amount_of_files, incorrect_path) {
    std::string wrong_path= "./wrong_dir";
    EXPECT_EXIT(get_amount_of_files(&wrong_path[0]), ::testing::ExitedWithCode(EXIT_FAILURE), "Error with opening dir");
}

TEST(get_amount_of_files, correct_amount_of_files) {
    std::string path_to_dir = "../../test_files";
    EXPECT_EQ(get_amount_of_files(&path_to_dir[0]), 500);
}
