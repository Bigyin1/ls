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
void print_file(t_args *args, struct stat st){
    printf("%s", args->curr_path);
}

void print_dir_content(t_args *args)
{
    for (int i = 0; i < args->files.len; ++i) {
        printf("%s", (char*)args->files.data[i]);
        if (i != args->files.len-1) printf("\n");
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
            if ((strcmp(curr_files.data[i], ".") != 0 && strcmp(curr_files.data[i], "..") != 0) ||
                    is_entrypoint || args->all) {
                if (i >= 1) printf("\n");
                if (!is_entrypoint || curr_files.len > 1) printf("%s:\n", args->curr_path);
                process_dir(args);
            }
            remove_last_path_elem(args);
            continue;
        }
        if (is_entrypoint) print_file(args, st);
        remove_last_path_elem(args);
    }
    free(curr_files.data);
}

void list(t_args *args)
{
    print_files(args, true);
}
