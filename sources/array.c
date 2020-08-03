//
// Created by sergey on 03.08.2020.
//

#include "array.h"
#include <stdlib.h>

t_array append(t_array arr, void* d)
{
    if (arr.cap == 0) {
        arr.cap = 2;
        arr.data = malloc(arr.cap*sizeof(void*));
    }
    if (arr.cap == arr.len) {
        arr.cap *= 2;
        arr.data = realloc(arr.data, arr.cap*sizeof(void*));
        if (arr.data == NULL) {
            exit(2);
        }
    }
    arr.data[arr.len++] = d;
    return arr;
}