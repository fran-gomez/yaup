#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>

#include <errno.h>
#include <err.h>

#include <sys/types.h>
#include <sys/stat.h>


void print_help();
mode_t parse_mode(const char *arg);
int chmod_rec(const char *path, mode_t mode);

int main(int argc, char const *argv[], char const *envp[]) {
    
    int i, rflag;
    mode_t mode;
    struct stat st;

    if (argc < 3)
        print_help();

    // Parse the arguments
    if (argv[1][0] == '-' && argv[1][1] == 'r') {
        rflag = 1;
        i = 3;
        mode = parse_mode(argv[2]);
        if (mode < 0)
            errx(EINVAL, "%s: invalid mode\n", argv[2]);
    } else {
        rflag = 0;
        i = 2;
        mode = parse_mode(argv[1]);
        if (mode < 0)
            errx(EINVAL, "%s: invalid mode\n", argv[2]);
    }

    while (i < argc) {
        stat(argv[i], &st);
        if (st.st_mode == mode) {
            fprintf(stdout, "mode of '%s' retained as '%s'\n", argv[i], rflag? argv[2]:argv[1]);
            i++;
            continue;
        }

        if (!rflag) {
            if (chmod(argv[i], mode) < 0)
                err(errno, "%s", argv[i]);
        } else {
            if (chmod_rec(argv[i], mode) < 0)
                err(errno, "%s", argv[i]);
        }

        i++;
    }

    return 0;
}

int chmod_rec(const char *path, mode_t mode) {

    DIR *dp;
    struct dirent *d;

    if ((dp=opendir(path)) == NULL)
        return -1;

    chdir(path);
    for (d = readdir(dp); d != NULL; d = readdir(dp)) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        
        if (d->d_type == DT_DIR)
            if (chmod_rec(d->d_name,  mode) < 0)
                return -1;
    
        if (chmod(path, mode) < 0)
            return -1;
    }
    chdir("..");

    return 0;
}

/*
 * Parse the octal mode passed as a string, and return its 
 * integer decimal value.
 * Return -1 if error
 */
mode_t parse_mode(const char *arg) {

    int d, i, l;
    mode_t mode = 0;

    l = strlen(arg);
    i = 0;
    while (l >= 0) {
        if (arg[l] < '0' || arg[l] > '7') 
            return -1;

        d = arg[l-1] - '0'; // Integer value of last digit
        mode += d * pow(8, i);
        i++; 
        l--; // Set the cursor to prev digit
    }

    return mode;
}

void print_help() {

    fputs("usage: chmod mode file...\n\n", stderr);

    fputs("Change the mode of each file\n", stderr);
    fputs("Each mode has the form 0-user-group-others, and in octal form\n", stderr);

    exit(EINVAL);
}