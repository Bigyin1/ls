#include "list.h"
#include "array.h"
#include <stdio.h>
#include "utils.h"
#include "errors.h"
#include "file.h"
#include <sys/dir.h>
#include "permission.h"


void print_access_err(t_ls *args)
{
    fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
    perror("");
    set_exit_code(args, ERR_MINOR);
    if (args->root_args) set_exit_code(args, ERR_FATAL);
}

void process_dir(t_ls *args)
{
    DIR* dir;
    struct dirent *de;
    t_array next_files = {0};

    if ((dir = opendir(args->curr_path)) == NULL) {
        print_access_err(args);
        return;
    }
    while ((de = readdir(dir)) != NULL) {
        t_file* file = new_file(args, de->d_name, args->is_long || args->sort_by_time || (de->d_type == DT_UNKNOWN));
        if (file == NULL) continue;
        file->type = de->d_type;
        if (args->is_long) file->perms = get_file_permissions(file->st.st_mode);
        next_files = append(next_files, file);
    }
    closedir(dir);
    args->files = next_files;
    sort_files(args);

    print_dir_content(args, args->files, false);
    if (args->recursive) {
        args->root_args = false;
        process_dirs(args);
        return;
    }
    free_file_arr(args->files);
}

void process_dirs(t_ls *args)
{
    t_file *f;
    t_array curr_dirs = args->files;

    for (int i = 0; i < curr_dirs.len ; ++i) {
        f = curr_dirs.data[i];
        if (get_file_type(f) != DT_DIR) continue;
        if (is_dot(f) && !args->root_args) continue;

        add_path_elem(args, f->name);
        if (!is_hidden(f) || args->root_args || args->print_all) {
            if (args->prev_files) printf("\n");
            if (args->recursive || args->prev_files) printf("%s:\n", args->curr_path);
            args->prev_files = true;
            process_dir(args);
        }
        remove_last_path_elem(args);
    }
    free_file_arr(curr_dirs);
}
