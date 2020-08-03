#include "list.h"
#include "parse_args.h"
#include "errors.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "utils.h"

void print_files(t_args *args, bool is_entrypoint);

struct s_print_meta {
    int pad_syml;
    int pad_usr;
    int pad_gr;
    int pad_sz;
    int pad_day;
    int blocks;
};




int print_file_info(struct stat st, char* file)
{

}

void print_file(t_args *args, char* file_name)
{
    struct stat st;

    add_path_elem(args, file_name);
    if (lstat(args->curr_path, &st) != 0) {
        fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
        perror("");
        args->exitCode = ERR_FATAL;
        remove_last_path_elem(args);
        return;
    }
    if (!args->is_long) {
        if (S_ISDIR(st.st_mode) != 0) {
            printf("%s%s%s", KBLU, file_name, KNRM);
            remove_last_path_elem(args);
            return;
        }
        printf("%s", file_name);
        remove_last_path_elem(args);
        return;
    }


    remove_last_path_elem(args);
}

void print_dir_content(t_args *args)
{
    for (int i = 0; i < args->files.len; ++i) {
        if (!args->print_all && is_hidden((char*)args->files.data[i])) continue;
        print_file(args, (char*)args->files.data[i]);
        if (!args->is_long && i != args->files.len-1) {
            printf("  ");
            continue;
        }
    }
    printf("\n");
}

void process_dir(t_args *args)
{
    DIR* dir;
    struct dirent *de;
    t_array f_names_arr = {0};

    if ((dir = opendir(args->curr_path)) == NULL) {
        perror("ls: ");
        args->exitCode = ERR_FATAL;
        return;
    }

    args->files = f_names_arr;
    while ((de = readdir(dir)) != NULL) {
        args->files = append(args->files, de->d_name);
    }
    sort_args(args);
    print_dir_content(args);
    if (args->recursive) {
        print_files(args, false);
    } else {
        free(args->files.data);
    }
    closedir(dir);
}

void print_files(t_args *args, bool is_entrypoint)
{
    struct stat st;
    t_array curr_files = args->files;

    for (int i = 0; i < curr_files.len ; ++i) {
        add_path_elem(args, curr_files.data[i]);

        if (lstat(args->curr_path, &st) != 0) {
            fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
            perror("");
            args->exitCode = ERR_FATAL;
            remove_last_path_elem(args);
            continue;
        }
        if (S_ISDIR(st.st_mode) != 0) {
            if (!is_hidden(curr_files.data[i]) || is_entrypoint || args->print_all) {
                if (i >= 1) printf("\n");
                if (args->recursive || curr_files.len > 1) printf("%s:\n", args->curr_path);
                process_dir(args);
            }
            remove_last_path_elem(args);
            continue;
        }
        if (is_entrypoint) print_file(args, args->curr_path);
        remove_last_path_elem(args);
    }
    free(curr_files.data);
}

void list(t_args *args)
{
    print_files(args, true);
}
