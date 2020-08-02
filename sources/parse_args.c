#include "parse_args.h"
#include "errors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static int parse_file_arg(t_args *ls_a, char* argv) {
    char** f = ls_a->files;

    for (; *f;f++);
    *f = argv;
    return NO_ERR;
}

static int parse_dash_arg(t_args *ls_a, char* argv)
{
    for (;*argv;argv++) {
        if (*argv == 'l') {ls_a->is_long = true; continue;}
        if (*argv == 'R') {ls_a->recursive = true;continue;}
        if (*argv == 'a') {ls_a->all = true;continue;}
        if (*argv == 'r') {ls_a->reverse = true;continue;}
        if (*argv == 't') {ls_a->sort_by_time = true;continue;}
        fprintf(stderr,
                "ls: invalid key - <<%c>>\nCheck out <<ls --help>>\n",
                *argv);
        return ERR_FATAL;
    }
    return NO_ERR;
}

static int parse_ddash_arg(t_args *ls_a, const char* argv) {

    if (!*argv) return true;
    if (!strcmp(argv, "all")) {ls_a->is_long = true; return 0;}
    if (!strcmp(argv, "recursive")) {ls_a->recursive = true; return 0;}
    if (!strcmp(argv, "reverse")) {ls_a->reverse = true; return 0;}
    fprintf(stderr,
            "ls: invalid param - <<%s>>\nCheck out <<ls --help>>\n",
            argv);
    return ERR_FATAL;
}


static int parse_arg(t_args *ls_a, char* argv)
{
    if (argv[0] == '-') {
        return parse_dash_arg(ls_a, argv+1);
    }
    if (!strncmp(argv, "--", 2)) {
        return parse_ddash_arg(ls_a, argv+2);
    }
    return parse_file_arg(ls_a, argv);
}

int parse_args(t_args *ls_a, int argc, char** argv)
{
    int err;

    if (!(ls_a->files = calloc(argc+1, sizeof(char*)))) {
        perror("ls:");
        return ERR_FATAL;
    }
    for (;*argv;argv++) {
        if ((err = parse_arg(ls_a, *argv) != 0)) {
            return err;
        }
    }
    return NO_ERR;
}