cmake_minimum_required(VERSION 3.17)
project(ls C)

set(CMAKE_C_STANDARD 11)

add_library(ls_lib
        parse_args.c
        list.c array.c
        utils.c permission.c file.c)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_options(ls_lib PRIVATE -fsanitize=address)
    target_link_libraries(ls_lib asan)
endif()