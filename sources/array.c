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
        void* n = realloc(arr.data, arr.cap*sizeof(void*));
        if (n == NULL) {
            free(arr.data);
            exit(2);
        }
        arr.data = n;
    }
    arr.data[arr.len++] = d;
    return arr;
}