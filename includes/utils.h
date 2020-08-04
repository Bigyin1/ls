//
// Created by sergey on 03.08.2020.
//

#ifndef LS_UTILS_H
#define LS_UTILS_H

#include "list.h"
#include "parse_args.h"

void sort_args(t_args*);
void add_path_elem(t_args *args, char* file);
void remove_last_path_elem(t_args *args);
bool is_hidden(char* file);
t_array filter_files(t_args *args);
void set_exit_code(t_args *args, int ecode);

#endif //LS_UTILS_H
