cmake_minimum_required(VERSION 3.0.0)
project(gaokao-admission-emulator-gcc VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(gaokao-admission-emulator-gcc ../admission_operations.c ../file_operations.c ../main.c ../merge_sort.c ../efiles/efiles.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
