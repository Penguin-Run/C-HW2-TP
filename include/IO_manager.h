#ifndef C_HW2_TP_IO_MANAGER_H
#define C_HW2_TP_IO_MANAGER_H

#define NUM_OF_DIFF_COUNT 11

#include "work.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include<time.h>

int generate_random_sequence(char* sequence, int num_of_bytes);
void generate_random_file(int num_of_bytes);
void generate_file(int num_of_bytes);

int work_from_file(const char* filename_input, const char* filename_output);
int work(char* region, size_t file_size, const char* filename_output);

#endif //C_HW2_TP_IO_MANAGER_H
