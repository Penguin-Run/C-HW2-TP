//
// Created by Иван on 24.10.2020.
//

#ifndef C_HW2_TP_IO_MANAGER_H
#define C_HW2_TP_IO_MANAGER_H

#include <stddef.h>

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

size_t get_file_size(char* filename); // !
char* load_file(char* filename);
void print_bytes(char* string);
void generate_file(char* filename);


#endif //C_HW2_TP_IO_MANAGER_H
