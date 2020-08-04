#include "parse_args.h"
#include <stdlib.h>
#include "list.h"
#include "utils.h"
#include "file.h"

int main(int argc, char** argv)
{
    t_ls args = {0};

    args.root_args = true;
    parse_args(&args, argc-1, argv+1);
    sort_files(&args);

    print_dir_content(&args, args.files, true);

    process_dirs(&args);
    exit(args.exitCode);
}
