//
// Created by Иван on 24.10.2020.
//

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

void work_from_file(const char* filename_input, const char* filename_output);

#endif //C_HW2_TP_IO_MANAGER_H
