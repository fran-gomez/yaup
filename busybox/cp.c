#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include <dirent.h>

#include <sys/stat.h>


void print_help(void);
int dir_to_dir(const char *src, const char *dst);
void file_to_dir(const char *src, const char *dst);
void file_to_file(const char *src, const char *dst);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i, rflag;
    struct stat st;

    if (argc < 3)
        print_help();
    
    // Check if we want to do a recursive copy
    if (argv[1][0] == '-') {
        if (argv[1][1] == 'r') {
            rflag = 1;
            i = 2;
        } else
            err(EINVAL, "%s", argv[1]);
    } else {
        rflag = 0;
        i = 1;
    }

    if (argc == 3) // File to file copy
        file_to_file(argv[1], argv[2]);
    else { // argc > 3, multiple sources to directory
        stat(argv[argc-1], &st);
        if (!S_ISDIR(st.st_mode))
            err(EINVAL, "%s", argv[argc-1]);

        while (i < argc-1) {
            stat(argv[i], &st);

            if (S_ISREG(st.st_mode))
                file_to_dir(argv[i], argv[argc-1]);
                

            if (S_ISDIR(st.st_mode)) {
                if (!rflag)
                    err(EINVAL, "%s", argv[i]);
                else
                    dir_to_dir(argv[i], argv[argc-1]);
            }

            i++;
        }
        
    }
    
    

    return 0;
}

// This part is common to each copy, open the files,
// read a char from src, write it to dst, repeat until eof(src)
// and close both files
void file_to_file(const char *src, const char *dst) {

    int c;
    FILE *fsrc, *fdst;

    fsrc = fopen(src, "r");
    if (!fsrc)
        err(errno, "%s", src);

    fdst = fopen(dst, "w");
    if (!fdst)
        err(errno, "%s", dst);

    c = fgetc(fsrc);
    while (c != EOF) {
        fputc(c, fdst);
        c = fgetc(fsrc);
    }

    fclose(fsrc);
    fclose(fdst);
}

// To copy a file into a directory, append the path of
// the directory before the path of the source
// (new_dst = dst/src), and make a file to file copy
// with the new destination file path
void file_to_dir(const char *src, const char *dst) {

    int dst_len;
    char *new_dst;

    new_dst = (char*) malloc(128);

    dst_len = strlen(dst);
    if (dst[dst_len-1] != '/')
        sprintf(new_dst, "%s/%s", dst, src);
    else
        sprintf(new_dst, "%s%s", dst, src);

    file_to_file(src, new_dst);
}

int dir_to_dir(const char *src, const char *dst) {
    
    int dst_len;
    char *new_dst;
    DIR *dp;
    struct dirent *d;

    dp = opendir(src);
    if (!dp)
        return -1;
    
    dst_len = strlen(dst);
    new_dst = (char*) malloc(128);
    if (dst[dst_len-1] != '/')
        sprintf(new_dst, "%s/%s", dst, src);
    else
        sprintf(new_dst, "%s%s", dst, src);

    chdir(src);
    // mkdir dst/src, dp->mode
    for (d=readdir(dp); d != NULL; d=readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;

        if (d->d_type == DT_REG)
            file_to_dir(d->d_name, dst);
        
        if (d->d_type == DT_DIR)
            if (dir_to_dir(d->d_name, dst) < 0)
                return -1;
    }
    chdir("..");

    return 0;
}

void print_help(void) {

    fputs("usage: cp [-r] file1 file2\n", stderr);
    fputs("       cp file ... directory\n\n", stderr);

    fputs("Copy file to file, or multiple files to directory\n", stderr);

    exit (EINVAL);
}