#include <stdio.h>
#include <time.h>

#include <errno.h>

#include "sh.h"

int yash_who(char **argv) {

    fprintf(stdout, "%s logged at [HOSTNAME] [DATE]\n", envp_value[WHO]);

    return 0;
}