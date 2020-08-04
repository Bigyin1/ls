//
// Created by sergey on 04.08.2020.
//

#ifndef LS_FILE_H
#define LS_FILE_H

#include "array.h"
#include "parse_args.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct s_file {
    char* name;
    struct stat st;
    u_char type;
    bool is_stat;
} t_file;

void print_dir_content(t_ls *args, t_array files, bool non_dir);
t_file* new_file(t_ls* args, char* name, bool w_stat);
u_char get_file_type(t_file* file);
bool is_hidden(t_file* file);
bool is_dot(t_file* file);



#endif//LS_FILE_H
