//
// Created by sergey on 03.08.2020.
//

#ifndef LS_LIST_H
#define LS_LIST_H

#include "list.h"
#include "parse_args.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void process_dirs(t_ls *args);
void print_access_err(t_ls *args);


#endif //LS_LIST_H
