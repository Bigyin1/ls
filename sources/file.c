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
#include <sys/types.h>
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
    }
    f->name = strdup(name);
    if (f->name == NULL) exit(ERR_FATAL);
    return f;
}

u_char get_file_type(t_file* file)
{
    if (file->type) return file->type;

    mode_t st_mode = file->st.st_mode;
    if ((st_mode & S_IFMT) == S_IFBLK)
        file->type = DT_BLK;
    else if ((st_mode & S_IFMT) == S_IFCHR)
        file->type = DT_CHR;
    else if ((st_mode & S_IFMT) == S_IFDIR)
        file->type = DT_DIR;
    else if ((st_mode & S_IFMT) == S_IFIFO)
        file->type = DT_FIFO;
    else if ((st_mode & S_IFMT) == S_IFLNK)
        file->type = DT_LNK;
    else if ((st_mode & S_IFMT) == S_IFSOCK)
        file->type = DT_SOCK;
    else if ((st_mode & S_IFMT) == S_IFREG)
        file->type = DT_REG;
    else
        file->type = DT_UNKNOWN;
    return file->type;
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

    u_char ft = get_file_type(f);
    if (ft == DT_DIR) return KBLU;
    return KNRM;
}

void print_symlink_content(t_ls *args, t_file* f, bool corr)
{
    char buf[PATH_MAX] = {0};
    if (!readlink(args->curr_path, buf, PATH_MAX)) return;

    printf(" -> ");
    char* txtcolor = corr ? get_file_color(f, args) : KRED;
    char *stop_color = args->color ? KNRM : "";
    if (args->color){
        printf("%s", txtcolor);
        if (!corr) printf("%s", BACKWHITE);
    }

    printf("%s%s", buf, stop_color);
}

void print_symlink(t_ls *args, t_file* f)
{
    bool corr = true;
    if (stat(args->curr_path, &f->st) != 0){
        corr = false;
    }

    char *txtcolor = corr ? KCYN : KRED;
    char *stop_color = args->color ? KNRM : "";
    if (args->color){
        printf("%s", txtcolor);
        if (!corr) printf("%s", BACKWHITE);
    }

    printf("%s%s", f->name, stop_color);
    if (args->is_long) {
        print_symlink_content(args, f, corr);
    }
}

void print_filename(t_ls *args, t_file* f)
{
    if (get_file_type(f) == DT_LNK){
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
    int printed = 0;
    t_file* f;

    if (args->is_long && !non_dir) {
        int bl = count_blocks(args, files);
        if (bl) printf("total %d\n", bl);
    }
    for (int i = 0; i < files.len; ++i) {
        f = (t_file*)files.data[i];
        if (non_dir && get_file_type(f) == DT_DIR) continue;
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
