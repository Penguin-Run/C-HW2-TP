#include "include/IO_manager.h"

#define FILENAME_INPUT "../test_data/book.txt"

// TODO: пофиксить утечки valgrind в тестах и в main

int main() {

    work_from_file(FILENAME_INPUT, NULL); // NULL == stdout by default

    return 0;
}