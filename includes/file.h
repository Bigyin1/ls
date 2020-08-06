//
// Created by sergey on 04.08.2020.
//

#ifndef LS_FILE_H
#define LS_FILE_H

#include "array.h"
#include "list.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31;1m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34;1m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36;1m"

#define BACKWHITE "\x1b[47;1m"
#define BACKNRM    "\x1b[0m"

typedef struct s_file {
    struct stat st;
    char* name;
    char* perms;
    u_char type;
} t_file;

void print_dir_content(t_ls *args, t_array files, bool non_dir);
t_file* new_file(t_ls* args, char* name, bool w_stat);
u_char get_file_type(t_file* file);
bool is_hidden(t_file* file);
bool is_dot(t_file* file);



#endif//LS_FILE_H
