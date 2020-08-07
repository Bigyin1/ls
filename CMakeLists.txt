cmake_minimum_required(VERSION 3.17)
project(ls C)

set(CMAKE_C_STANDARD 11)

include_directories(includes)
add_subdirectory(sources)
add_subdirectory(test)


add_executable(ls main.c)
target_link_libraries(ls ls_lib)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_options(ls PRIVATE -fsanitize=address)
    target_link_libraries(ls asan)
endif()
