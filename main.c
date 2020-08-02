#include <stdio.h>
#include "errors.h"
#include "parse_args.h"

int main(int argc, char** argv) {
    t_args args = {0};
    int ec;

    if ((ec = parse_args(&args, argc-1, argv+1)) != NO_ERR) {
        return ec;
    }


//    for (;*args.files;args.files++) {
//        printf("%s\n", *args.files);
//    }
    return 0;
}
