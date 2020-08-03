#include <stdio.h>
#include "errors.h"
#include "parse_args.h"
#include "list.h"
#include "utils.h"

int main(int argc, char** argv) {
    t_args args = {0};
    int ec;

    if ((ec = parse_args(&args, argc-1, argv+1)) != NO_ERR) {
        return ec;
    }

    sort_args(&args);
    list(&args);
    return args.exitCode;
}
