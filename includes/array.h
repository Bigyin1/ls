//
// Created by sergey on 03.08.2020.
//

#ifndef LS_ARRAY_H
#define LS_ARRAY_H

#include <stddef.h>
#include "errors.h"

typedef struct s_array
{
    void **data;
    size_t cap, len;
} t_array;

t_array append(t_array arr, void* d);
t_array new_arr(int len, int cap);

#endif //LS_ARRAY_H
