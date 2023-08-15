cmake_minimum_required(VERSION 3.25)
project(assembler_project C)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -ansi -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(assembler_project
        assembler.c
        parse_macros.c first_pass_through.c symbol_table.c symbol_table.h utils.c utils.h first_pass_through.h parse_macros.h assembler.h processing_helpers.c processing_helpers.h instruction_handling.c instruction_handling.h error_detection_pass.c error_detection_pass.h second_pass_through.c second_pass_through.h encoding_functions.c encoding_functions.h create_output_files.c create_output_files.h)