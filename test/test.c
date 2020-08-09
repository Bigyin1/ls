#include "parse_args.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int saved_stdout;

int capture_stdout() {
    int out_pipe[2];

    saved_stdout = dup(STDOUT_FILENO);

    if (pipe(out_pipe) != 0) {
        exit(1);
    }

    dup2(out_pipe[1], STDOUT_FILENO);
    return out_pipe[0];
}

void restore_stdout() {
    dup2(saved_stdout, STDOUT_FILENO);
}


void test_parse_args() {
    t_ls ls = {0};
    char *argv[] = {"-laR", "-r", "-1", "./test_dir", NULL};
    parse_args(&ls, argv);

    assert(ls.files.len == 1);
    assert(ls.is_long);
    assert(ls.one_col);
    assert(ls.print_all);
    assert(ls.recursive);
    assert(ls.reverse);
    free_file_arr(ls.files);
    printf("test_parse_args: OK\n");
}


void test_print_simple() {
    int r_fd = capture_stdout();
    char buffer[32] = {0};

    t_ls ls = {0};
    char *argv[] = {"./test_dir", NULL};
    list(&ls, argv);
    read(r_fd, buffer, 32);

    assert(strcmp(buffer, "test2\n") == 0);
    restore_stdout();
    printf("test_print_simple: OK\n");
}

void test_print_recursive() {
    int r_fd = capture_stdout();
    char buffer[128] = {0};

    t_ls ls = {0};
    char *argv[] = {"-R", "./test_dir", "./test_dir/test2/test.txt", NULL};
    list(&ls, argv);
    read(r_fd, buffer, 128);

    assert(strcmp(buffer, "./test_dir/test2/test.txt\n"
                          "\n"
                          "./test_dir:\n"
                          "test2\n"
                          "\n"
                          "./test_dir/test2:\n"
                          "test.txt\n") == 0);
    restore_stdout();
    printf("test_print_recursive: OK\n");
}


int main() {

    test_parse_args();
    test_print_simple();
    test_print_recursive();

    return EXIT_SUCCESS;
}
