//
// Created by sergey on 04.08.2020.
//

#ifndef LS_PERMISSION_H
#define LS_PERMISSION_H

#include <sys/stat.h>

char get_file_type_char(mode_t st_mode);
char *get_file_permissions(mode_t st_mode);

#endif//LS_PERMISSION_H
