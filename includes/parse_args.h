#ifndef LS_LS_H
#define LS_LS_H

#include <stdbool.h>
#include <limits.h>
#include "array.h"

typedef struct s_args
{
    // file list
    t_array files;

    char curr_path[PATH_MAX];

    // opts
    bool is_long;
    bool recursive;
    bool print_all;
    bool reverse;
    bool sort_by_time;

    int exitCode;
}   t_args;

int parse_args(t_args *ls_a, int args, char** argv);




#endif //LS_LS_H