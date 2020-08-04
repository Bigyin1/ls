//
// Created by sergey on 03.08.2020.
//

#ifndef LS_UTILS_H
#define LS_UTILS_H

#include <stdbool.h>
#include <parse_args.h>

void sort_files(t_ls *args);
void add_path_elem(t_ls *args, char* file);
void remove_last_path_elem(t_ls *args);
void set_exit_code(t_ls *args, int ecode);

#endif //LS_UTILS_H
