#include "gtest/gtest.h"
#include <fstream>

extern "C" {
#include "../include/IO_manager.h"
}

// TODO: настроить замер времени работы через chrono

TEST(Test_work_from_file_func, small_file){
    const char* input_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/small_file";
    const char* output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/test_results/small_file_results";
    const char* expected_output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/expected_results/small_file_results";
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
    const char* input_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/book.txt";
    const char* output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/test_results/book.txt_results";
    const char* expected_output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/expected_results/book.txt_results";
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

#define MEGABYTE_IN_BYTES 1000000
TEST(Test_work_from_file_func, big_file_100mb_generated) {
    // TODO: сделать еще одну функцию генерации файла где будет рандом каждый раз
    //generate_file(MEGABYTE_IN_BYTES*100);
    const char* input_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/generated_file";
    const char* output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/test_results/generated_file_results";
    const char* expected_output_filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/expected_results/generated_file_results";
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