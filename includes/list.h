//
// Created by sergey on 03.08.2020.
//

#ifndef LS_LIST_H
#define LS_LIST_H


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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void print_dir_content(t_args *args);
void process_dirs(t_args *args);


#endif //LS_LIST_H
