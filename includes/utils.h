//
// Created by sergey on 03.08.2020.
//

#ifndef LS_UTILS_H
#define LS_UTILS_H

#include "list.h"

void sort_args(t_args*);
void add_path_elem(t_args *args, char* file);
void remove_last_path_elem(t_args *args);
bool is_hidden(char* file);

#endif //LS_UTILS_H
