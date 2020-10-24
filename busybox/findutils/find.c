#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include <dirent.h>

#include <sys/stat.h>


void print_help();
int find(const char *name, const char *root, char *path);


int main(int argc, char const *argv[], char const *envp[]) {
    
    char *path;
    struct stat st;

    if (argc != 3)
        print_help();

    if (stat(argv[2], &st) < 0)
        err(errno, "%s", argv[2]);

    if (!S_ISDIR(st.st_mode))
        errx(ENOTDIR, "%s: %s\n", argv[2], strerror(ENOTDIR));
    
    path = (char*) malloc(sizeof(char)*128);
    if (find(argv[1], argv[2], path))
        fputs(path, stdout);
    else
        fprintf(stdout, "%s not found\n", argv[1]);        

    return 0;
}

// Look for name in dp. If not found, try to find name
// in the children directories of dp
int find(const char *name, const char *root, char *path) {

    DIR *dp;
    char aux[128];
    struct dirent *d;

    if (!(dp=opendir(root)))
        return -1;
    
    chdir(root);
    for (d=readdir(dp); d != NULL; d=readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        
        if (strcmp(d->d_name, name) != 0) {
            if (d->d_type == DT_DIR)
                if (find(name, d->d_name, path))
                    sprintf(path, "%s/%s", root, path);
        } else {
            strcat(path, d->d_name);
            return 1;
        }
    }
    chdir("..");

    return 0;
}

void print_help() {

    fputs("usage: find src root\n\n", stderr);

    fputs("123\n", stderr);

    exit(EINVAL);
}