#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help(void);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int sflag = 0;

    if (argc < 3)
        print_help();

    if (argc == 4) {
        if (argv[1][0] == '-' && argv[1][1] == 's')
            sflag = 1;
        else
            errx(EINVAL, "%s: invalid option\n", argv[1]);
    }

    if (sflag) {
        if (symlink(argv[2], argv[3]) < 0)
            err(errno, "%s, %s", argv[2], argv[3]);
    } else {
        if (link(argv[2], argv[3]) < 0)
            err(errno, "%s, %s", argv[2], argv[3]);
    }

    return 0;
}


void print_help(void) {

    fputs("usage: ln [-s] name1 [name2]\n\n", stderr);

    fputs("Make a link from name1 to name2\n", stderr);

    exit(EINVAL);
}