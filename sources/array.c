//
// Created by sergey on 03.08.2020.
//

#include "array.h"
#include <stdlib.h>

t_array append(t_array arr, void* d)
{
    if (arr.cap == 0) {
        arr.cap = 128;
        arr.data = malloc(arr.cap*sizeof(void*));
    }
    if (arr.cap == arr.len) {
        arr.cap *= 2;
        void* n = realloc(arr.data, arr.cap*sizeof(void*));
        if (n == NULL) {
            free(arr.data);
            exit(ERR_FATAL);
        }
        arr.data = n;
    }
    arr.data[arr.len++] = d;
    return arr;
}

t_array new_arr(int len, int cap)
{
    t_array arr = {.len = len, .cap = cap};

    arr.data = (void**)calloc(cap, sizeof(void*));
    if (arr.data == NULL) exit(ERR_FATAL);
    return arr;
}

void free_arr(t_array arr, bool free_cont)
{
    if (free_cont) {
        for (int i = 0; i < arr.len; ++i) {
            free(arr.data[i]);
        }
    }
    free(arr.data);

}