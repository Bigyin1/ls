//
// Created by sergey on 03.08.2020.
//

#ifndef LS_LIST_H
#define LS_LIST_H

#include "array.h"
#include "list.h"
#include <limits.h>
#include <stdbool.h>


typedef struct s_ls {
    // file list
    t_array files;

    bool printed_line;
    bool root_args;

    char curr_path[PATH_MAX];

    // opts
    bool is_long;
    bool recursive;
    bool print_all;
    bool reverse;
    bool sort_by_time;
    bool one_col;
    bool color;

    int exit_code;
} t_ls;


void process_dirs(t_ls *args, bool root);
void print_access_err(t_ls *args);


#endif//LS_LIST_H
