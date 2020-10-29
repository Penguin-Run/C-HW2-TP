#include "../include/work.h"

#include <stdio.h>

void find_diff(char* region, size_t file_size, int* diff_count, int num_of_diff) {
    printf( "CONSECUTIVE running..\n");
    for (int i = 0; i < num_of_diff; i++) {
        int count = 0;
        int j = 1;
        while(region[j]) {
            if (abs(region[j] - region[j-1]) == i)
                count++;
            j++;
        }
        diff_count[i] = count;
    }
}


