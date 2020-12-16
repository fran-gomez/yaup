#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help(void);
char *new_path(const char *p1, const char *p2);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i, rflag;
    struct stat st;

    if (argc < 3)
        print_help();
    
    if (argv[1][0] == '-') {
        if (argv[1][1] == 'r') {
            i = 2; rflag = 1;
        } else
            errx(EINVAL, "'%s' is not a valid argument\n", argv[1]);
    } else {
        rflag = 0;
        i = 1;
    }

    if (argc == 3) {
        if (rename(argv[1], argv[2]) < 0)
            err(errno, "%s", argv[1]);
    } else {
        stat(argv[argc-1], &st);
        if (!S_ISDIR(st.st_mode))
            errx(ENOTDIR, "'%s' must be a directory\n", argv[argc-1]);

        while (i < argc-1) {
            if (stat(argv[i], &st) < 0)
                err(errno, "%s", argv[i]);

            // Move file to dst/file
            if (S_ISREG(st.st_mode))
                if (rename(argv[i], new_path(argv[argc-1], argv[i])) < 0)
                    err(errno, "%s", argv[argc-1]);

            // Move dir/* to dst/dir/
            if (S_ISDIR(st.st_mode)) {
                if (rflag)
                    errx(EINPROGRESS, "Not yet implemented\n");
                else
                    errx(EINVAL, "-r: Option not set");
            }

            i++;
        }
    }

    return 0;
}

char *new_path(const char *p1, const char *p2) {

    char *newpath;

    newpath = (char*) malloc(sizeof(char) * 512);
    sprintf(newpath, "%s/%s", p1, p2);

    return newpath;
}

void print_help(void) {

    fputs("usage: mv file1 file2\n", stderr);
    fputs("       mv [-r] name ... dst\n\n", stderr);

    fputs("Move or rename file and directories\n", stderr);
    exit(EINVAL);
}