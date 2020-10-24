#include "../include/IO_manager.h"
#include "../include/parallel_work.h"
#include "../include/consecutive_work.h"

#define NUM_OF_DIFF 11
// TODO: убрать thread_routine из хэдэра ведь он не паблик (или все таки не получится?)
// TODO: в тестах настроить замер времени работы через chrono

int main() {
    // char* filename = "/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/book.txt";
    char* filename = "/Users/penguin/TPark-SEM1/C-HW2-TP/test_data/book.txt";

    char* region = load_file(filename); // allocate mmap!
    int size = (int) get_file_size(filename);
    printf("Size of file: %d\n", size);
    // print_bytes(region);

    int* diff_count = calloc(NUM_OF_DIFF, sizeof(int));

    // find_diff_consecutive(region, diff_count, NUM_OF_DIFF);
    find_diff_parallel(region, get_file_size(filename), diff_count, NUM_OF_DIFF);

    for (int i = 0; i < NUM_OF_DIFF; i++) printf("Pairs with byte difference %d: %d\n", i, diff_count[i]);

    free(diff_count);
    // очищение mmap памяти
    if (munmap(region, get_file_size(filename)) != 0) {
        printf("munmap failed\n");
    }

    return 0;
}