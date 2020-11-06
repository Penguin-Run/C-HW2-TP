#ifndef C_HW2_TP_IO_MANAGER_H
#define C_HW2_TP_IO_MANAGER_H

#define NUM_OF_DIFF_COUNT 11

#include "work.h"



int generate_random_sequence(char* sequence, int num_of_bytes);
void generate_random_file(int num_of_bytes);
void generate_file(int num_of_bytes);
int compare_files(const char* file_1, const char* file_2); // -1: error; 0: equal; 1: not equal;
int generate_unique_filename(const char* base, size_t base_size, size_t salt_size, char* result);

int work_from_file(const char* filename_input, const char* filename_output);
int work(char* region, size_t file_size, const char* filename_output);

#endif //C_HW2_TP_IO_MANAGER_H
