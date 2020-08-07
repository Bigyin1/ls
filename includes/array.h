//
// Created by sergey on 03.08.2020.
//

#ifndef LS_ARRAY_H
#define LS_ARRAY_H


#include <stddef.h>

typedef struct s_array {
    void **data;
    size_t cap, len;
} t_array;

t_array append(t_array arr, void *d);
t_array filter_files(t_array arr);
t_array filter_dirs(t_array arr);
void free_file_arr(t_array arr);

#endif//LS_ARRAY_H
