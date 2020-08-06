#include "file.h"
#include "utils.h"
#include <stdlib.h>
#include "errors.h"
#include "list.h"
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "unistd.h"


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
        f->type = f->st.st_mode & S_IFMT;
    }
    f->name = strdup(name);
    if (f->name == NULL) exit(ERR_FATAL);
    return f;
}

bool is_dot(t_file* file)
{
    return (strcmp(file->name, ".") == 0 || strcmp(file->name, "..") == 0);
}

bool is_hidden(t_file* file)
{
    return (strncmp(file->name, ".", 1) == 0 || strncmp(file->name, "..", 2) == 0);
}

/*********************** print files *****************************/


int count_blocks(t_ls *args, t_array files)
{
    int res = 0;

    for (int i = 0; i < files.len; ++i) {
        t_file *f = ((t_file*)files.data[i]);
        if (is_hidden(f) && !args->print_all) continue;
        res += f->st.st_blocks/2;
    }
    return res;
}

// doesn't return color for symlink
char* get_file_color(t_file *f, t_ls *args)
{
    if (!args->color) return "";

    if (f->type == S_IFDIR) return KBLU;
    return KNRM;
}

void print_symlink_content(t_ls *args, t_file* f, bool valid)
{
    char buf[PATH_MAX] = {0};
    if (!readlink(args->curr_path, buf, PATH_MAX)) return;

    printf(" -> ");
    if (args->color){
        printf("%s", valid ? get_file_color(f, args) : KRED);
        if (!valid) printf("%s", BACKWHITE);
    }

    printf("%s%s", buf, args->color ? KNRM : "");
}

void print_symlink(t_ls *args, t_file* f)
{
    bool valid = true;
    if (stat(args->curr_path, &f->st) != 0){
        valid = false;
        f->type = f->st.st_mode&S_IFMT;
    }

    if (args->color){
        printf("%s", valid ? KCYN : KRED);
        if (!valid) printf("%s", BACKWHITE);
    }

    printf("%s%s", f->name, args->color ? KNRM : "");
    if (args->is_long) {
        print_symlink_content(args, f, valid);
    }
}

void print_filename(t_ls *args, t_file* f)
{
    if (f->type == S_IFLNK){
        print_symlink(args, f);
        return;
    }
    char *stop_color = args->color ? KNRM : "";
    printf("%s%s%s", get_file_color(f, args), f->name, stop_color);
}

void print_file_meta(t_ls *args, t_file* f) {
    struct passwd *usr = getpwuid(f->st.st_uid);
    if (usr == NULL) return;
    struct group *gr = getgrgid(f->st.st_gid);
    if (gr == NULL) return;
    struct tm *time = gmtime(&f->st.st_mtim.tv_sec);

    char t[6] = {0};
    sprintf(t, "%02d:%02d", time->tm_hour, time->tm_min);
    printf("%s %d %s %s %10d %-4s %d %s ", f->perms,
           (int) f->st.st_nlink,
           usr->pw_name, gr->gr_name,
           (int) f->st.st_size,
           get_month_verbose(time->tm_mon),
           time->tm_mday, t);
}

void print_file(t_ls* args, t_file* f) {
    add_path_elem(args, f->name);
    if (args->is_long) {
        print_file_meta(args, f);
    }
    print_filename(args, f);
    remove_last_path_elem(args);
}

void print_dir_content(t_ls *args, t_array files, bool non_dir)
{
    int     printed = 0;
    t_file* f;

    if (args->is_long && !non_dir) {
        int bl = count_blocks(args, files);
        if (bl) printf("total %d\n", bl);
    }
    for (int i = 0; i < files.len; ++i) {
        f = (t_file*)files.data[i];
        if (non_dir && f->type == S_IFDIR) continue;
        if (!args->print_all && is_hidden(f)) continue;

        print_file(args, f);
        args->prev_files = true;
        if (!args->is_long && !args->one_col && i != args->files.len-1) {
            printf("  ");
            continue;
        }
        if (i != args->files.len-1) {
            printf("\n");
            continue;
        }
        ++printed;
    }
    if (printed) printf("\n");
}
