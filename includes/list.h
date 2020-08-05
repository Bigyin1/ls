//
// Created by sergey on 03.08.2020.
//

#ifndef LS_LIST_H
#define LS_LIST_H

#include "array.h"
#include "list.h"
#include <limits.h>
#include <stdbool.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef struct s_ls {
    // file list
    t_array files;

    bool prev_files;
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

    int exitCode;
}   t_ls;


void process_dirs(t_ls *args);
void print_access_err(t_ls *args);


#endif //LS_LIST_H
