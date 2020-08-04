//
// Created by sergey on 04.08.2020.
//

#include "file.h"
#include "utils.h"
#include <stdlib.h>
#include "errors.h"
#include "list.h"
#include <string.h>
#include <sys/dir.h>
#include <stdio.h>

t_file* new_file(t_ls* args, char* name, bool w_stat)
{
    t_file *f = calloc(sizeof(t_file), 1);
    if (f == NULL) exit(ERR_FATAL);
    if (w_stat) {
        struct stat st;
        add_path_elem(args, name);
        if (lstat(args->curr_path, &st) != 0) {
            print_access_err(args);
            remove_last_path_elem(args);
            free(f);
            return NULL;
        }
        remove_last_path_elem(args);
        f->st = st;
        f->is_stat = true;
    }
    f->name = strdup(name);
    if (f->name == NULL) exit(ERR_FATAL);
    return f;
}

u_char get_file_type(t_file* file)
{
    if (!file->is_stat) return file->type;

    if (S_ISDIR(file->st.st_mode)) return DT_DIR;
    if (S_ISREG(file->st.st_mode)) return DT_REG;
    return DT_UNKNOWN;
}

bool is_dot(t_file* file)
{
    return (strcmp(file->name, ".") == 0 || strcmp(file->name, "..") == 0);
}

bool is_hidden(t_file* file)
{
    return (strncmp(file->name, ".", 1) == 0 || strncmp(file->name, "..", 2) == 0);
}

int print_file_info(t_ls *args, t_file* f)
{
    add_path_elem(args, f->name);
    if (!args->is_long) {
        if (get_file_type(f) == DT_DIR) {
            printf("%s%s%s", KBLU, f->name, KNRM);
            remove_last_path_elem(args);
            return true;
        }
        printf("%s", f->name);
        remove_last_path_elem(args);
        return false;
    }
}

bool print_file(t_ls* args, t_file* f)
{
    print_file_info(args, f);
    return 0;
}

void print_dir_content(t_ls *args, t_array files, bool non_dir)
{
    int printed = 0;
    for (int i = 0; i < files.len; ++i) {
        t_file* f = (t_file*)files.data[i];
        if (non_dir && get_file_type(f) == DT_DIR) continue;
        if (!args->print_all && is_hidden(f)) continue;
        print_file(args, f);
        args->prev_files = true;
        if (!args->is_long && i != args->files.len-1) {
            printf("  ");
            continue;
        }
        ++printed;
    }
    if (printed) printf("\n");
}
