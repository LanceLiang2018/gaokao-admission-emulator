cmake_minimum_required(VERSION 3.17)
project(gaokao_admission_emulator C)

set(CMAKE_C_STANDARD 99)

add_executable(gaokao_admission_emulator main.c file_operations.c admission_elements.h merge_sort.c admission_operations.c)