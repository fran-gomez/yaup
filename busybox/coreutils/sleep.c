#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

int yash_sleep(char **argv) {

    unsigned int secs;
    unsigned int left;

    if (argv[1] == NULL) {
        fprintf(stderr, "usage: sleep [SECS]\n");
        return EINVAL;
    }

    secs = atoi(argv[1]);
    left = sleep(secs);
    if (left > 0) {
        fprintf(stderr, "sleep: %s\n", strerror(errno));
        return EINTR;
    }

    return 0;
}