cmake_minimum_required(VERSION 3.17)
project(ls C)

set(CMAKE_C_STANDARD 11)


add_executable(ls sources/main.c
        sources/parse_args.c
        sources/list.c sources/array.c
        sources/utils.c sources/permission.c sources/formatting.c)
include_directories(includes)
target_compile_options(ls PRIVATE -O3)
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_options(ls PRIVATE -fsanitize=address)
    target_link_libraries(ls asan)
endif()
