cmake_minimum_required(VERSION 3.15)
project(C_HW2_TP C)

set(CMAKE_C_STANDARD 11)

##
### Test definitions ###
##




##
### Source definitions ###
##

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(C_HW2_TP ${sources})