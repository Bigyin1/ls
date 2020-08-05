#include <stdlib.h>
#include "list.h"
#include "utils.h"
#include "file.h"
#include "parse_args.h"
#include "errors.h"

int main(int argc, char** argv)
{
    t_ls args = {0};

    args.root_args = true;
    if (parse_args(&args, argc-1, argv+1) != NO_ERR) exit(ERR_FATAL);
    sort_files(&args);
    print_dir_content(&args, args.files, true);

    process_dirs(&args);
    exit(args.exitCode);
}
