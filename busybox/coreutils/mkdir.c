#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <err.h>


void print_help();
int mkdir_parent(const char *path, mode_t mask);
mode_t get_mask();

int main(int argc, char const *argv[]) {
    
    int i, pflag;
    mode_t umask = get_mask();
    char dir_ent[256];

    if (argc < 2)
        print_help();
    
    if (argv[1][0] == '-' && argv[1][1] == 'p') {
        pflag = 1; i = 2;
    } else {
        pflag = 0; i = 1;
    }

    for (; i < argc; i++)
        if (pflag) {
            if (mkdir(argv[i], umask) < 0)
                err(errno, "'%s'", argv[i]);
        } else {
            if (mkdir_parent(argv[i], umask) < 0)
                err(errno, "'%s'", argv[i]);
        }

    return 0;
}

mode_t get_mask() {

    int c;
    FILE *status;
    char pid_path[32];
    mode_t mask = 0000;
    pid_t pid = getpid();

    sprintf(pid_path, "/proc/%d/status", pid);
    status = fopen(pid_path, "r");
    if (status == NULL)
        err(errno, "'%s'", pid_path);
    
    // Skip first line, umask is on second line
    // fmt = Name: <proc name>
    while ((c=fgetc(status)) != '\n');

    // Get the mask from the second line
    // fmt = Umask: <mask>
    while ((c=fgetc(status)) != ':');
    while ((c=fgetc(status)) != '\n') {
        mask = mask*10 + (c - '0');
    }

    fclose(status);

    return mask;
}

int mkdir_parent(const char *path, mode_t mask) {
    return -EINPROGRESS;
}

void print_help() {
    fputs("usage: mkdir [-p] directory\n\n", stderr);

    fputs("Create a directory\n", stderr);
    fputs("Option -p create the whole directory tree\n", stderr);

    exit(EINVAL);
}