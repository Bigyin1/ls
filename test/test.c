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


void test_parse_args() {

    t_ls ls = {0};
    char *argv[] = {"-laR", "-r", "-1", "./test", NULL};
    parse_args(&ls, argv);

    assert(ls.files.len == 1);
    assert(ls.is_long);
    assert(ls.one_col);
    assert(ls.print_all);
    assert(ls.recursive);
    assert(ls.reverse);
    free_file_arr(ls.files);
    printf("test_parse_args: OK");
}

void test_print() {

}

int main() {

    test_parse_args();

    return EXIT_SUCCESS;
}
