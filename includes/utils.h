#ifndef LS_UTILS_H
#define LS_UTILS_H

#include "list.h"
#include <stdbool.h>

void sort_files(t_ls *args);
void add_path_elem(t_ls *args, char *file);
void remove_last_path_elem(t_ls *args);
void remove_all_path(t_ls *args);
void set_exit_code(t_ls *args, int ecode);
char *get_month_verbose(int md);

#endif//LS_UTILS_H
