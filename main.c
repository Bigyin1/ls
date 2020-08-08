#include "list.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    t_ls args = {0};
    list(&args, argv + 1);
    exit(args.exit_code);
}
