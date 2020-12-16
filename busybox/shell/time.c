#include <stdio.h>
#include <time.h>

#include <errno.h>

#include <sys/types.h>

#include "sh.h"


int select_cmd(char const *builtins[], const char *cmd);


int yash_time(char **argv) {
    
    int i, status;
    time_t begin, end, total;

    if (argv[1] == NULL) {
        fprintf(stderr, "usage: time [CMD]...\n");
        fprintf(stderr, "Time a given command\n");
        return EINVAL;
    }

    begin = clock();

    argv++;
    i = select_cmd(builtins, *argv);
    if (i == -1)
        status = (*functions[11])(argv);
    else if (i >= 0)
        status = (*functions[i])(argv);
    else
        fprintf(stderr, "Command not found");

    if (status == 0) {
        end = clock();
        total = end - begin;
        fprintf(stdout, "%ld msecs\n", total);
    }
    
    return status;
}