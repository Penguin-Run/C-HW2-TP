//
// Created by Иван on 24.10.2020.
//

#ifndef C_HW2_TP_PARALLEL_WORK_H
#define C_HW2_TP_PARALLEL_WORK_H

#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

typedef struct data_chunk {
    char* data;
    size_t size;
    int num_of_diff;
    int* diff_count;
} data_chunk;

void* thread_routine(void* arg);
void find_diff_parallel(char* region, size_t file_size, int* diff_count, int num_of_diff);


#endif //C_HW2_TP_PARALLEL_WORK_H
