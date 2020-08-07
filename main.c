#include "errors.h"
#include "file.h"
#include "list.h"
#include "parse_args.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    t_ls args = {0};
    args.root_args = true;
    if (parse_args(&args, argv + 1) != NO_ERR) exit(ERR_FATAL);
    sort_files(&args);

    t_array f = filter_files(args.files);
    print_dir_content(&args, f, true);
    remove_all_path(&args);
    free_file_arr(f);

    t_array d = filter_dirs(args.files);
    args.files = d;
    process_dirs(&args, true);
    exit(args.exit_code);
}
