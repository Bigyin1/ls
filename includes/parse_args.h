#ifndef LS_LS_H
#define LS_LS_H

#include <stdbool.h>

typedef struct s_args
{
    // file list
    char** files;

    // opts
    bool is_long;
    bool recursive;
    bool all;
    bool reverse;
    bool sort_by_time;
}   t_args;

int parse_args(t_args *ls_a, int args, char** argv);



#endif //LS_LS_H