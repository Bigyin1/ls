#include "list.h"
#include "array.h"
#include "errors.h"
#include "file.h"
#include "permission.h"
#include "utils.h"
#include <stdio.h>
#include <sys/dir.h>


void print_access_err(t_ls *args) {
    fprintf(stderr, "ls: can't access '%s': ", args->curr_path);
    perror("");
    set_exit_code(args, ERR_MINOR);
    if (args->root_args) set_exit_code(args, ERR_FATAL);
}

void process_dir(t_ls *args) {
    DIR *dir;
    struct dirent *de;
    t_array next_files = {0};

    if ((dir = opendir(args->curr_path)) == NULL) {
        print_access_err(args);
        return;
    }
    while ((de = readdir(dir)) != NULL) {
        add_path_elem(args, de->d_name);
        t_file *file = new_file(args, de->d_name, true);
        remove_last_path_elem(args);
        if (file == NULL) continue;
        if (args->is_long) file->perms = get_file_permissions(file->st.st_mode);
        next_files = append(next_files, file);
    }
    closedir(dir);
    args->files = next_files;
    sort_files(args);

    print_dir_content(args, args->files, false);
    if (args->recursive) {
        process_dirs(args, false);
        return;
    }
    free_file_arr(args->files);
}

void process_dirs(t_ls *args, bool root) {
    t_file *f;
    t_array curr_dirs = args->files;

    for (int i = 0; i < curr_dirs.len; ++i) {
        f = curr_dirs.data[i];
        if (f->type != S_IFDIR) continue;
        if (is_dot(f) && !root) continue;
        if (root) args->root_args = true;

        add_path_elem(args, f->name);
        if (!is_hidden(f) || args->print_all || root) {
            if (args->printed_line) printf("\n");
            if (args->printed_line ||
                args->recursive ||
                curr_dirs.len > 1) printf("%s:\n", args->curr_path);
            args->printed_line = true;
            process_dir(args);
        }
        if (root) remove_all_path(args);
        remove_last_path_elem(args);
    }
    free_file_arr(curr_dirs);
}
