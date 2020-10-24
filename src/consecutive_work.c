#include "../include/consecutive_work.h"

void find_diff_consecutive(char* region, int* diff_count, int num_of_diff) {
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


