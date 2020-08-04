#include "errors.h"
#include "parse_args.h"
#include "list.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
    t_args args = {0};
    int ec;

    if ((ec = parse_args(&args, argc-1, argv+1)) != NO_ERR) {
        return ec;
    }
    args.root_args = true;
    sort_args(&args);
    t_array files = filter_files(&args, true);
    t_array dirs = args.files;

    args.files = files;
    print_dir_content(&args, false);
    free_arr(args.files, true);
    if (args.files.len) args.prev_files = true;


    args.files = dirs;
    process_dirs(&args);
    exit(args.exitCode);
}
