#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <errno.h>
#include <err.h>

#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

void print_help(void);
void error(const char *arg);

gid_t get_gid(const char *str);
int chgrp_rec(char const *path, gid_t gid);

int main(int argc, char const *argv[], char const *envp[]) {

    int i, rflag;
    gid_t gid;
    struct stat st;

    if (argc < 3)
        print_help();

    // Check if we have to apply a recursive change
    if (argv[1][0] == '-' && argv[1][1] == 'r') {
        rflag = 1;
        i = 3;
        gid = get_gid(argv[2]);
        if (gid < 0) 
            errx(EINVAL, "'%s' is not a valid group\n", argv[2]);
    } else {
        rflag = 0;
        i = 2;
        gid = get_gid(argv[1]);
        if (gid < 0)
            errx(EINVAL, "'%s' is not a valid group\n", argv[1]);
    }

    while (i < argc) {
        stat(argv[i], &st);
        if (gid == st.st_gid) {
            fprintf(stdout, "group ownership of '%s' retained as '%s'\n", argv[i], rflag? argv[2]:argv[1]);
            i++;
            continue;
        }

        if (!rflag) {
            if (chown(argv[i], st.st_uid, gid) < 0)
                err(errno, "%s", argv[i]);
        } else {
            if (chgrp_rec(argv[i], gid) < 0)
                err(errno, "%s", argv[i]);
        }
        
        i++;
    }

    return 0;
}

// FIX IT: Concatenar la ruta que precede al archivo, o cambiar el directorio actual de trabajo
int chgrp_rec(char const *path, gid_t gid) {

    DIR *dp;
    struct dirent *d;

    if ((dp=opendir(path)) == NULL)
        return -1;
    
    // Get into the directory 'getcwd()/path'
    chdir(path);
    for (d = readdir(dp); d != NULL; d = readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;

        // If entry is a directory, apply the change recursively
        if (d->d_type == DT_DIR)
            if (chgrp_rec(d->d_name, gid) < 0)
                return -1;

        // Change the group ownership of the entry
        if (chown(d->d_name, -1, gid) < 0)
            return -1;
        
    }
    // Go back to the base directory
    chdir("..");

    return 0;
}

void print_help(void) {

    fputs("usage: chgrp [-r] group file...\n\n", stderr);

    fputs("Change the group owner for each file or directory\n", stderr);
    fputs("If -r option is passed, apply the change recursively to each directory.\n", stderr);

    exit(EINVAL);
}

void get_delim(char *buff, int sz, int delim, FILE *str) {

    int c, i = 0;

    c = fgetc(str);
    while (c != EOF && c != delim) {
        buff[i++] = c;
        c = fgetc(str);

        if (i > sz)
            return;
    }
    
    buff[i] = '\0';
}

gid_t get_gid(const char *str) {

    int found = 0;
    char gname[64], gpass[512], gid[5];
    FILE *groups;

    groups = fopen("/etc/group", "r");
    if (!groups)
        err(errno, "/etc/group");

    while (!feof(groups)) {
        get_delim(gname, 64, ':', groups);
        
        if (strcmp(gname, str) == 0) {
            get_delim(gpass, 512, ':', groups);
            get_delim(gid, 5, ':', groups);
            found = 1;
        } else
            get_delim(gpass, 512, '\n', groups);
    }

    if (!found) 
        return -1;
    else
        return atoi(gid);
}