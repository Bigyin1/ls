cmake_minimum_required(VERSION 3.17)

project(ls C)

add_executable(ls_tests_run test.c)
target_link_libraries(ls_tests_run ls_lib)
file(COPY test_dir DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_options(ls_tests_run PRIVATE -fsanitize=address)
    target_link_libraries(ls_tests_run asan)
endif()