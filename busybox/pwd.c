#include <stdio.h>
#include <unistd.h>

#include "sh.h"

int yash_pwd(char **argv) {

    fprintf(stdout, "%s\n", envp_value[PWD]);
    
    return 0;
}