//
// Created by sergey on 02.08.2020.
//

#ifndef LS_ERRORS_H
#define LS_ERRORS_H

#define NO_ERR 0
#define ERR_MINOR 1
#define ERR_FATAL 2

typedef struct s_error
{
    int err_code;
    char* err_verbose;
} t_error;


#endif //LS_ERRORS_H
