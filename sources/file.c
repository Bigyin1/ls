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

    mode_t st_mode = file->st.st_mode;
    if ((st_mode & S_IFMT) == S_IFBLK)
        return DT_BLK;
    else if ((st_mode & S_IFMT) == S_IFCHR)
        return DT_CHR;
    else if ((st_mode & S_IFMT) == S_IFDIR)
        return DT_DIR;
    else if ((st_mode & S_IFMT) == S_IFIFO)
        return DT_FIFO;
    else if ((st_mode & S_IFMT) == S_IFLNK)
        return DT_LNK;
    else if ((st_mode & S_IFMT) == S_IFSOCK)
        return DT_SOCK;
    else if ((st_mode & S_IFMT) == S_IFREG)
        return DT_REG;
    else
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


/*********************** print files *****************************/



void print_file_short(t_ls *args, t_file* f)
{
    if (args->color) {
        if (get_file_type(f) == DT_DIR) {
            printf("%s%s%s", KBLU, f->name, KNRM);
            return;
        }
    }
    printf("%s", f->name);
}

void print_file_long(t_ls *args, t_file* f) {
    struct passwd *usr = getpwuid(f->st.st_uid);
    if (usr == NULL) return;
    struct group *gr = getgrgid(f->st.st_gid);
    if (gr == NULL) return;
    struct tm *time = gmtime(&f->st.st_mtim.tv_sec);

    char t[6] = {0};
    sprintf(t, "%02d:%02d", time->tm_hour, time->tm_min);
    printf("%s %d %s %s %10d %s %d %s ", f->perms,
           (int) f->st.st_nlink,
           usr->pw_name, gr->gr_name,
           (int) f->st.st_size,
           get_month_verbose(time->tm_mon),
           time->tm_mday, t);
}

void print_file(t_ls* args, t_file* f) {
    if (!args->is_long) {
        print_file_short(args, f);
        return;
    }
    print_file_long(args, f);
    print_file_short(args, f);
}

void print_dir_content(t_ls *args, t_array files, bool non_dir)
{
    int printed = 0;
    if (args->is_long && !non_dir) {
        int bl = count_blocks(args, files);
        if (bl) printf("total %d\n", bl);
    }
    for (int i = 0; i < files.len; ++i) {
        t_file* f = (t_file*)files.data[i];
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
