cmake_minimum_required(VERSION 3.14)
project(Lab3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Lab3 main.c Core/io.c Auxiliar/binary_ops.c Auxiliar/binary_ops.h Auxiliar/custom_string_ops.c Auxiliar/custom_string_ops.h Core/structs.h Core/register.c Core/register.h Core/ram.c Core/ram.h Core/instr_mem.c Core/instr_mem.h Auxiliar/linked_list.c Auxiliar/linked_list.h Core/cpu.c Core/cpu.h)

target_link_libraries(Lab3 m)