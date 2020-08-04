//
// Created by sergey on 03.08.2020.
//

#include "utils.h"

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
    return strcasecmp(*(const char**)val1, *(const char**)val2);
}

static int by_name_r( const void * val1, const void * val2 )
{
    return strcasecmp(*(const char**)val2, *(const char**)val1);
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

bool is_hidden(char* file)
{
    return (strncmp(file, ".", 1) == 0 || strncmp(file, "..", 2) == 0);
}

void set_exit_code(t_args *args, int ecode)
{
    if (ecode <= args->exitCode) return;
    args->exitCode = ecode;
}

t_array filter_files(t_args *args){
    t_array f_arr = {0};
    t_array d_arr = {0};
    struct stat st;

    for (int i = 0; i < args->files.len; ++i) {
        add_path_elem(args, args->files.data[i]);
        if (lstat(args->curr_path, &st) == -1) {
            fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
            perror("");
            set_exit_code(args, ERR_MINOR);
            if (args->root_args) set_exit_code(args, ERR_FATAL);
            remove_last_path_elem(args);
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            d_arr = append(d_arr, args->files.data[i]);
        } else {
            f_arr = append(f_arr, args->files.data[i]);
        }
        remove_last_path_elem(args);
    }
    free(args->files.data);
    args->files = d_arr;
    return f_arr;
}
