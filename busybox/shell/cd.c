#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include "sh.h"

int yash_cd(char **argv) {

    if (chdir(argv[1]) < 0) {
        fprintf(stderr, "cd: %s: %s\n", argv[1], strerror(errno));
        return errno;
    }

    //strncpy(envp_value[PWD], getcwd(NULL, 0),256);

    return 0;
}