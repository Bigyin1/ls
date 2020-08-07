#include "errors.h"
#include "file.h"
#include <stdlib.h>

t_array append(t_array arr, void *d) {
    if (arr.cap == 0) {
        arr.cap = 64;
        arr.data = malloc(arr.cap * sizeof(void *));
    }
    if (arr.cap == arr.len) {
        arr.cap *= 2;
        void *n = realloc(arr.data, arr.cap * sizeof(void *));
        if (n == NULL) {
            free(arr.data);
            exit(ERR_FATAL);
        }
        arr.data = n;
    }
    arr.data[arr.len++] = d;
    return arr;
}

t_array filter_files(t_array arr) {
    t_array files = {0};
    for (int i = 0; i < arr.len; ++i) {
        t_file *f = (t_file *) arr.data[i];
        if (f->type != S_IFDIR) files = append(files, f);
    }
    return files;
}


t_array filter_dirs(t_array arr) {
    t_array dirs = {0};
    for (int i = 0; i < arr.len; ++i) {
        t_file *f = (t_file *) arr.data[i];
        if (f->type == S_IFDIR) dirs = append(dirs, f);
    }
    return dirs;
}

void free_file_arr(t_array arr) {
    for (int i = 0; i < arr.len; ++i) {
        t_file *f = (t_file *) arr.data[i];
        free(f->name);
        free(f->perms);
        free(f);
    }
    free(arr.data);
}
