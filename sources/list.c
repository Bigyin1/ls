#include "list.h"



int print_file_info(struct stat st, char* file)
{

}

bool print_file(t_args *args, char* file_name)
{
    struct stat st;

    add_path_elem(args, file_name);
    if (lstat(args->curr_path, &st) != 0) {
        fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
        perror("");
        set_exit_code(args, ERR_MINOR);
        if (args->root_args) set_exit_code(args, ERR_FATAL);
        remove_last_path_elem(args);
        return false;
    }
    if (!args->is_long) {
        if (S_ISDIR(st.st_mode) != 0) {
            printf("%s%s%s", KBLU, file_name, KNRM);
            remove_last_path_elem(args);
            return true;
        }
        printf("%s", file_name);
        remove_last_path_elem(args);
        return false;
    }
    remove_last_path_elem(args);
    return 0;
}

t_array print_dir_content(t_args *args, bool filter_dirs)
{
    t_array filtered_d = {0};
    for (int i = 0; i < args->files.len; ++i) {
        if (!args->print_all && is_hidden((char*)args->files.data[i])) continue;
        if(strlen((char*)args->files.data[i]) == 0) continue;
        if (print_file(args, (char*)args->files.data[i]) && filter_dirs) {
            filtered_d = append(filtered_d, strdup((char*)args->files.data[i]));
        }
        if (!args->is_long && i != args->files.len-1) {
            printf("  ");
            continue;
        }
    }
    if (args->files.len) printf("\n");
    return filtered_d;
}

void process_dir(t_args *args)
{
    DIR* dir;
    struct dirent *de;
    t_array f_names_arr = {0};

    if ((dir = opendir(args->curr_path)) == NULL) {
        fprintf(stderr,"ls: can't access '%s': ", args->curr_path);
        perror("");
        set_exit_code(args, ERR_MINOR);
        if (args->root_args) set_exit_code(args, ERR_FATAL);
        return;
    }

    args->files = f_names_arr;
    while ((de = readdir(dir)) != NULL) {
        args->files = append(args->files, strdup(de->d_name));
    }
    sort_args(args);
    if (args->recursive) {
        t_array dirs = print_dir_content(args, true);
        free_arr(args->files, true);
        args->files = dirs;
        args->root_args = false;
        process_dirs(args);
    } else {
        print_dir_content(args, false);
        free_arr(args->files, true);
    }
    closedir(dir);
}

void process_dirs(t_args *args)
{
    t_array curr_dirs = args->files;

    for (int i = 0; i < curr_dirs.len ; ++i) {
        add_path_elem(args, curr_dirs.data[i]);
        if (!is_hidden(curr_dirs.data[i]) || args->root_args || args->print_all) {
            if (args->prev_files) printf("\n");
            if (args->recursive || args->prev_files) printf("%s:\n", args->curr_path);
            args->prev_files = true;
            process_dir(args);
        }
        remove_last_path_elem(args);
    }
    free_arr(curr_dirs, true);
}
