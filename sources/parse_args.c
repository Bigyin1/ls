#include "parse_args.h"
#include "errors.h"
#include "file.h"
#include "list.h"
#include <permission.h>
#include <stdio.h>
#include <string.h>

static int parse_file_arg(t_ls *ls_a, char* argv)
{
    t_file *f = new_file(ls_a, argv, true);
    if (f == NULL) return 0;
    if (ls_a->is_long) f->perms = get_file_permissions(f->st.st_mode);
    ls_a->files = append(ls_a->files, f);
    return NO_ERR;
}

static int parse_dash_arg(t_ls *ls_a, char* argv)
{
    for (;*argv;argv++) {
        if (*argv == 'l') {ls_a->is_long = true; continue;}
        if (*argv == 'R') {ls_a->recursive = true;continue;}
        if (*argv == 'a') {ls_a->print_all = true;continue;}
        if (*argv == 'r') {ls_a->reverse = true;continue;}
        if (*argv == 't') {ls_a->sort_by_time = true;continue;}
        if (*argv == '1') {ls_a->one_col = true;continue;}
        fprintf(stderr,
                "ls: invalid key - <<%c>>\nCheck out <<ls --help>>\n",
                *argv);
        return ERR_FATAL;
    }
    return NO_ERR;
}

static int parse_ddash_arg(t_ls *ls_a, const char* argv)
{

    if (!*argv) return true;
    if (!strcmp(argv, "all")) {ls_a->is_long = true; return 0;}
    if (!strcmp(argv, "recursive")) {ls_a->recursive = true; return 0;}
    if (!strcmp(argv, "reverse")) {ls_a->reverse = true; return 0;}
    fprintf(stderr,
            "ls: invalid param - <<%s>>\nCheck out <<ls --help>>\n",
            argv);
    return ERR_FATAL;
}

static int parse_arg(t_ls *ls_a, char* argv)
{
    if (argv[0] == '-') {
        return parse_dash_arg(ls_a, argv+1);
    }
    if (!strncmp(argv, "--", 2)) {
        return parse_ddash_arg(ls_a, argv+2);
    }
    return parse_file_arg(ls_a, argv);
}

static int check_stdout(t_ls *ls_a)
{
    struct stat st;
    if (fstat(fileno(stdout), &st) != 0) {
        perror("");
        return ERR_FATAL;
    }
    if (!S_ISCHR(st.st_mode)) {
            ls_a->one_col = true;
            ls_a->color = false;
    }
    return NO_ERR;
}

int parse_args(t_ls *ls_a, int argc, char** argv)
{
    int err;

    ls_a->color = true;

    if (check_stdout(ls_a) != NO_ERR) return ERR_FATAL;
    for (;*argv;argv++) {
        if ((err = parse_arg(ls_a, *argv) != 0)) {
            return err;
        }
    }
    if (ls_a->files.len == 0 && ls_a->exitCode == NO_ERR) {
        t_file *f = new_file(ls_a, ".", true);
        if (f == NULL) return ERR_FATAL;
        ls_a->files = append(ls_a->files, f);

    }
    return NO_ERR;
}