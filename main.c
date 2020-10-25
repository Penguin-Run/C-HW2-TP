#include "include/IO_manager.h"
#include <sys/types.h>
#include <sys/sysctl.h>

#define MEGABYTE_IN_BYTES 1000000

int main() {

    // generate_file(100 * MEGABYTE_IN_BYTES);
    work_from_file("/Users/Ivan/TPark-SEM1/C-HW2-TP/test_data/generated_file", NULL); // NULL == stdout by default

    return 0;
}