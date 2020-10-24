#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include <errno.h>
#include <err.h>

#include <sys/stat.h>
#include <sys/types.h>

void print_help(void);
int chown_rec(const char *path, uid_t owner);
uid_t get_uid(const char *uid);

int main(int argc, char const *argv[], char const *envp[]) {
    
    uid_t uid;
    int i, rflag;
    struct stat st;

    if (argc < 3)
        print_help();
    
    if (argv[1][0] == '-' && argv[1][1] == 'r') {
        rflag = 1;
        i = 3;
        uid = get_uid(argv[2]);
        if (uid < 0)
            errx(EINVAL, "'%s' is not a valid user\n", argv[2]);
    } else {
        rflag = 0;
        i = 2;
        uid = get_uid(argv[1]);
        if (uid < 0)
            err(EINVAL, "'%s' is not a valid user\n", argv[1]);
    }
    
    while (i < argc) {
        stat(argv[i], &st);
        if (st.st_uid == uid) {
            fprintf(stdout, "ownership of '%s' retained as '%s'\n", argv[i], rflag? argv[2]:argv[1]);
            i++;
            continue;
        }

        if (!rflag) {
            if (chown(argv[i], uid, st.st_gid) < 0)
                err(errno, "%s\n", argv[i]);
        } else {
            if (chown_rec(argv[i], uid) < 0)
                err(errno, "%s\n", argv[i]);
        }
    }

    return 0;
}

int chown_rec(const char *path, uid_t owner) {
    
    DIR *dp;
    struct dirent *d;

    if ((dp=opendir(path)) == NULL)
        return -1;
    
    chdir(path);
    for (d = readdir(dp); d != NULL; d = readdir(dp)) {
        if (strcmp(".", d->d_name) == 0 || strcmp("..", d->d_name) == 0)
            continue;
        
        if (d->d_type == DT_DIR)
            if (chown_rec(d->d_name, owner) < 0)
                return -1;
        
        if (chown(path, owner, -1) < 0)
            return -1;
    }
    chdir("..");

    return 0;
}

void print_help(void) {

    fputs("usage: chown [-r] owner files...\n\n", stderr);

    fputs("Change the owner for each file\n", stderr);

    exit(EINVAL);
}

uid_t get_uid(const char *uid) {
    return 1000;
}