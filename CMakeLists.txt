cmake_minimum_required(VERSION 3.25)
project(assembler_project C)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -ansi -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(assembler_project
        assembler.c
        parse_macros.c first_pass_through.c symbol_table.c symbol_table.h)