#include "include/IO_manager.h"

#define FILENAME_INPUT "../test_data/book.txt"
#define FILENAME_OUTPUT "../test_data/test_results/book.txt_results"

// TODO: пофиксить утечки valgrind в тестах и в main

int main() {

    work_from_file(FILENAME_INPUT, FILENAME_OUTPUT); // NULL == stdout by default

    return 0;
}