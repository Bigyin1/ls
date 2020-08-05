//
// Created by sergey on 03.08.2020.
//

#include "utils.h"
#include "file.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

static int by_time( const void * val1, const void * val2)
{
    t_file* f1 = *(t_file** )val1;
    t_file* f2 = *(t_file** )val2;

    return (int)(f1->st.st_mtim.tv_nsec - f2->st.st_mtim.tv_nsec);
}

static int by_time_r( const void * val1, const void * val2 )
{
    t_file* f1 = *(t_file** )val1;
    t_file* f2 = *(t_file** )val2;


    return (int)(f2->st.st_mtim.tv_nsec - f1->st.st_mtim.tv_nsec);
}

static int by_name(const void * val1, const void * val2 )
{
    t_file* f1 = *(t_file** )val1;
    t_file* f2 = *(t_file** )val2;

    return strcasecmp(f1->name, f2->name);
}

static int by_name_r( const void * val1, const void * val2 )
{
    t_file* f1 = *(t_file** )val1;
    t_file* f2 = *(t_file** )val2;

    return strcasecmp(f2->name, f1->name);
}

void sort_files(t_ls* args)
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

void add_path_elem(t_ls *args, char* file)
{
    unsigned long p_len;

    p_len = strlen(args->curr_path);

    if (p_len != 0 && args->curr_path[p_len-1] != '/') {
        strcat(args->curr_path, "/");
    }
    strcat(args->curr_path, file);
}

void remove_last_path_elem(t_ls *args)
{
    unsigned long p_len;

    p_len = strlen(args->curr_path);
    if (p_len == 0) return;
    if (args->curr_path[p_len-1] == '/') args->curr_path[p_len-1] = 0;
    for (ulong i = p_len-1; i >= 0;) {
        if (args->curr_path[i] == '/') {
            if (i == 1 && args->curr_path[0] == '.') {
                args->curr_path[i] = 0;
                args->curr_path[0] = 0;
            }
            break;
        }
        args->curr_path[i] = 0;
        if (i == 0) break;
        --i;
    }
}

char* get_month_verbose(int md)
{
    switch (md) {
        case 0: return "jan";
        case 1: return "feb";
        case 2: return "mar";
        case 3: return "apr";
        case 4: return "may";
        case 5: return "jun";
        case 6: return "jul";
        case 7: return "aug";
        case 8: return "sep";
        case 9: return "oct";
        case 10: return "nov";
        case 11: return "dec";
        default: return "---";
    }
}

void set_exit_code(t_ls *args, int ecode)
{
    if (ecode <= args->exitCode) return;
    args->exitCode = ecode;
}
