cmake_minimum_required(VERSION 3.15)
project(C_HW2_TP C)

set(CMAKE_C_STANDARD 11)



add_executable(C_HW2_TP main.c src/IO_manager.c include/IO_manager.h src/parallel_work.c src/consecutive_work.c include/work.h)