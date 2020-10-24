#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help(void);
int  rm_dir(const char *root);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i, rflag;
    struct stat st;

    if (argc < 2)
        print_help();

    if (argv[1][0] == '-') {
        if (argv[1][1] == 'r') {
            rflag = 1; i = 2;
        } else
            errx(EINVAL, "'%s': Invalid argument\n", argv[1]);
    } else {
        rflag = 0; i = 1;
    }

    while (i < argc) {
        
        stat(argv[i], &st);

        if (S_ISREG(st.st_mode))
            if (remove(argv[i]) < 0)
                err(errno, "%s", argv[i]);
        
        if (S_ISDIR(st.st_mode)) {
            if (rflag) {
                if (rm_dir(argv[i]) < 0)
                    err(errno, "%s", argv[i]);
            } else
                errx(EINVAL, "-r: Option not set");
        }

        i++;
    }
    

    return 0;
}

int rm_dir(const char *root) {
    
    DIR *dp;
    struct dirent *d;

    if (!(dp=opendir(root)))
        return -1;
    
    for (d=readdir(dp); d != NULL; d=readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        
        if (d->d_type == DT_DIR)
            if (rm_dir(d->d_name) < 0)
                return -1;
        
        if (remove(d->d_name) < 0)
                return -1;
    }

    return 0;
}

void print_help(void) {

    fputs("usage: rm file...\n", stderr);
    fputs("       rm -r name...\n\n", stderr);

    fputs("Remove the files or directories (recursively)\n", stderr);

    exit(EINVAL);
}