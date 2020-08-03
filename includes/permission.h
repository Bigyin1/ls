//
// Created by sergey on 04.08.2020.
//

#ifndef LS_PERMISSION_H
#define LS_PERMISSION_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

char get_file_type(mode_t st_mode);
char *get_file_permissions(mode_t st_mode, char *filename);

#endif //LS_PERMISSION_H
