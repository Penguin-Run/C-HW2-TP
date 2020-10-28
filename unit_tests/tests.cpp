#include "gtest/gtest.h"
#include <fstream>

#include <dlfcn.h>

extern "C" {
#include "../include/IO_manager.h"
}

#define MEGABYTE_IN_BYTES 1000000

// work correctly with test_consecutive build
TEST(Stress_test, stress_test_100mb) {
    void *library; // объект для привязки внешней библиотеки
    int (*work_from_file_parallel)(const char* filename_input, const char* filename_output);


    // "./libparallel_work_lib.dylib" on mac
    // "./libparallel_work_lib.so" on linux
    library = dlopen("./libparallel_work_lib.so", RTLD_LAZY);
    if (!library) {
        fprintf(stderr, "library opening failed\n");
        ASSERT_TRUE(false);
    }

    // загрузка функции
    void* func = dlsym(library, "work_from_file");
    work_from_file_parallel = (int (*)(const char *, const char *)) func;


    // генерация файла случайных символов на 100 мегабайт
    generate_random_file(MEGABYTE_IN_BYTES * 100);
    const char* input_filename =
            "../test_data/generated_file_random";
    const char* output_filename_consecutive =
            "../test_data/test_results/stress_tests/results_consecutive";
    const char* output_filename_parallel =
            "../test_data/test_results/stress_tests/results_parallel";

    // вызов последовательной и параллельной реализации
    work_from_file(input_filename, output_filename_consecutive);

    (*work_from_file_parallel)(input_filename, output_filename_parallel);
    dlclose(library);

    std::ifstream consecutive_output(output_filename_consecutive);
    std::ifstream parallel_output(output_filename_parallel);
    if (!consecutive_output.is_open() || !parallel_output.is_open() || 1)
    {
        std::cerr << "TEST Failed to open file";
        consecutive_output.close();
        parallel_output.close();
        ASSERT_TRUE(false);
    }


    std::string line1;
    std::string line2;
    while (getline(consecutive_output, line1) && getline(parallel_output, line2)) {
        ASSERT_TRUE(line1 == line2);
    }

    consecutive_output.close();
    parallel_output.close();
}

TEST(Test_work_from_file_func, small_file){
    const char* input_filename = "../test_data/small_file";
    const char* output_filename = "../test_data/test_results/small_file_results";
    const char* expected_output_filename = "../test_data/expected_results/small_file_results";
    work_from_file(input_filename, output_filename);


    std::ifstream real_output(output_filename);
    std::ifstream expected_output(expected_output_filename);
    if (!real_output.is_open() || !expected_output.is_open())
    {
        std::cerr << "TEST Failed to open file";
        real_output.close();
        expected_output.close();
        ASSERT_TRUE(false);
    }


    std::string line1;
    std::string line2;
    while (getline(real_output, line1) && getline(expected_output, line2)) {
        ASSERT_TRUE(line1 == line2);
    }

    real_output.close();
    expected_output.close();
}


TEST(Test_work_from_file_func, medium_file) {
    const char* input_filename = "../test_data/book.txt";
    const char* output_filename = "../test_data/test_results/book.txt_results";
    const char* expected_output_filename = "../test_data/expected_results/book.txt_results";
    work_from_file(input_filename, output_filename);


    std::ifstream real_output(output_filename);
    std::ifstream expected_output(expected_output_filename);
    if (!real_output.is_open() || !expected_output.is_open())
    {
        std::cerr << "TEST Failed to open file";
        real_output.close();
        expected_output.close();
        ASSERT_TRUE(false);
    }


    std::string line1;
    std::string line2;
    while (getline(real_output, line1) && getline(expected_output, line2)) {
        ASSERT_TRUE(line1 == line2);
    }

    real_output.close();
    expected_output.close();
}

TEST(Test_work_from_file_func, big_file_100mb_generated) {
    // generate_file(MEGABYTE_IN_BYTES*100);
    /*
     * to generate new pseudo-random file and run test correctly:
     * - invoke generate_file(MEGABYTE_IN_BYTES*100); once
     * - paste result from any algorythm
     * from test_data/test_results/generated_file_results to test_data/expected_results/generated_file_results
     */
    const char* input_filename = "../test_data/generated_file";
    const char* output_filename = "../test_data/test_results/generated_file_results";
    const char* expected_output_filename = "../test_data/expected_results/generated_file_results";
    work_from_file(input_filename, output_filename);


    std::ifstream real_output(output_filename);
    std::ifstream expected_output(expected_output_filename);
    if (!real_output.is_open() || !expected_output.is_open())
    {
        std::cerr << "TEST Failed to open file";
        real_output.close();
        expected_output.close();
        ASSERT_TRUE(false);
    }


    std::string line1;
    std::string line2;
    while (getline(real_output, line1) && getline(expected_output, line2)) {
        ASSERT_TRUE(line1 == line2);
    }

    real_output.close();
    expected_output.close();
}

TEST(Test_work_from_file_func, incorrect_file_name) {
    const char* input_filename = "../test_data/unexisting_file";
    const char* output_filename = "../test_data/test_results/unexisting_file_results";

    int result = work_from_file(input_filename, output_filename);
    const int EXPECTED = -1;

    ASSERT_EQ(result, EXPECTED);
}


