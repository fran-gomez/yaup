#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <err.h>
#include <errno.h>

#include <dirent.h>

#include <sys/stat.h>


void print_help(void);
struct dirent **alpha_sort(DIR *dp);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i;
    DIR *dp;
    struct dirent **entries;
    struct stat st;

    if (argc < 2)
        print_help();

    if (argc == 2) {
        dp = opendir(argv[1]);
        if (!dp)
            err(errno, "%s", argv[1]);
        entries = alpha_sort(dp);

        while (strcmp((*entries)->d_name, "") != 0) {
            fprintf(stdout, "%s\n", (*entries)->d_name);
            entries++;
        }
        free(entries);

        closedir(dp);
    } else {
        for (i = 1; i < argc; i++) {
            stat(argv[i], &st);
            if (!S_ISDIR(st.st_mode))
                errx(ENOTDIR, "%s: must be a directory\n", argv[i]);

            dp = opendir(argv[i]);
            if (!dp)
                err(errno, "%s", argv[1]);
            
            fprintf(stdout, "%s:\n", argv[i]);
            entries = alpha_sort(dp);
            while (entries != NULL) {
                fprintf(stdout, "%s\n", (*entries)->d_name);
                entries++;
            }
            free(entries);

            closedir(dp);
        }
    }

    return 0;
}

struct dirent **alpha_sort(DIR *dp) {

    int i = 0;
    struct dirent **entries, *d;

    entries = (struct dirent **) malloc(sizeof(struct dirent*) * 512);
    if (!entries)
        return NULL;
    
    for (d=readdir(dp); d != NULL; d=readdir(dp)) {
        entries[i] = d;
        i++;
    }
    entries[i] = NULL;

    return entries;
}

void print_help(void) {

    fputs("usage: ls name...\n\n", stderr);

    fputs("List contents of directory\n", stderr);

    exit(EINVAL);
}