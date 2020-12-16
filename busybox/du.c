#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include <dirent.h>

#include <sys/stat.h>


void print_help(void);
size_t dir_du(const char *path);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i;
    double size;
    char unit;
    struct stat st;

    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &st) < 0)
            err(errno, "%s", argv[i]);

        if (S_ISREG(st.st_mode)) {
            size = st.st_size;

            if (size >= 0 && size <= 1024)
                unit = 'B';
            else if (size > 1024 && size <= 102400) {
                size /= 1024;
                unit = 'K';
            } else {
                size /= 1024;
                size /= 1024;
                unit = 'M';
            }
            fprintf(stdout, "%.1f%c \t %s\n", size, unit, argv[i]);
        }
            

        if (S_ISDIR(st.st_mode)) {
            size = dir_du(argv[i]);

            if (size >= 0 && size <= 1024)
                unit = 'B';
            else if (size > 1024 && size <= 102400) {
                size /= 1024;
                unit = 'K';
            } else {
                size /= 1024;
                size /= 1024;
                unit = 'M';
            }
            fprintf(stdout, "%.1f%c \t %s\n", size, unit, argv[i]);
        }
    }

    return 0;
}

size_t dir_du(const char *path) {
    
    DIR *dp;
    struct dirent *d;
    struct stat st;
    size_t dsize = 0;

    if (!(dp=opendir(path)))
        return -1;
    
    chdir(path);
    for (d = readdir(dp); d != NULL; d = readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        
        if (d->d_type == DT_DIR)
            dsize += dir_du(d->d_name);
        
        stat(path, &st);
        dsize += st.st_size;
    }
    chdir("..");

    return dsize;
}

void print_help(void) {

    fputs("usage: du name...\n\n", stderr);

    fputs("Summarize disk usage of name\n", stderr);

    exit(EINVAL);
}