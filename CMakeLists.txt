cmake_minimum_required(VERSION 3.17)
project(ls C)

set(CMAKE_C_STANDARD 11)

include_directories(includes)
add_executable(ls main.c sources/parse_args.c)