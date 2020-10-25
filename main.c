#include "include/IO_manager.h"
#include <sys/types.h>
#include <sys/sysctl.h>
int main() {

    // NULL == stdout by default
    work_from_file("/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/book.txt", NULL);



    return 0;
}