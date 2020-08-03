//
// Created by sergey on 03.08.2020.
//

#include "utils.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static int by_time( const void * val1, const void * val2)
{
    struct stat st1;
    struct stat st2;

    lstat(*(const char**)val1, &st1);
    lstat(*(const char**)val2, &st2);

    return (int)(st1.st_mtim.tv_nsec - st2.st_mtim.tv_nsec);
}

static int by_time_r( const void * val1, const void * val2 )
{
    struct stat st1;
    struct stat st2;

    lstat(*(const char**)val1, &st1);
    lstat(*(const char**)val2, &st2);

    return (int)(st1.st_mtim.tv_nsec - st2.st_mtim.tv_nsec);
}

static int by_name(const void * val1, const void * val2 )
{
    return strcmp(*(const char**)val1, *(const char**)val2);
}

static int by_name_r( const void * val1, const void * val2 )
{
    return strcmp(*(const char**)val2, *(const char**)val1);
}

void sort_args(t_args* args)
{
    if (args->sort_by_time) {
        args->reverse ? qsort(args->files.data, args->files.len, sizeof(void*), by_time_r) :
            qsort(args->files.data, args->files.len, sizeof(void*), by_time);
        return;
    }
    if (args->reverse){
        qsort(args->files.data, args->files.len, sizeof(void*), by_name_r);
        return;
    }
    qsort(args->files.data, args->files.len, sizeof(void*), by_name);
}

void add_path_elem(t_args *args, char* file)
{
    unsigned long p_len;

    p_len = strlen(args->curr_path);
    if (p_len != 0 && args->curr_path[p_len-1] != '/') {
        strcat(args->curr_path, "/");
    }
    strcat(args->curr_path, file);
}

void remove_last_path_elem(t_args *args)
{
    unsigned long p_len;

    p_len = strlen(args->curr_path);
    if (p_len == 0) return;
    if (args->curr_path[p_len-1] == '/') args->curr_path[p_len-1] = 0;
    for (ulong i = p_len-1; i >= 0;) {
        if (args->curr_path[i] == '/') break;
        args->curr_path[i] = 0;
        if (i == 0) break;
        --i;
    }
}
