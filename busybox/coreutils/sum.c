#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help(void);
int sum(FILE *fp);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int i;
    FILE *fp;
    struct stat st;

    if (argc < 2)
        print_help();

    for (i = 1; i < argc; i++) {
        stat(argv[i], &st);
        if (!S_ISREG(st.st_mode))
            errx(ENFILE, "%s, %s\n", argv[i], strerror(ENFILE));
        
        fp = fopen(argv[i], "r");
        if (!fp)
            err(errno, "%s", argv[i]);
        
        fprintf(stdout, "%d\t%ld %s\n", sum(fp), (st.st_size/st.st_blksize), argv[i]);
    }

    return 0;
}

int sum(FILE *fp) {

    int c;
    long checksum = 0;

    c = fgetc(fp);
    while (c != EOF) {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += c;
        checksum &= 0xffff;

        c = fgetc(fp);
    }

    return checksum;
}

void print_help(void) {

    fputs("usage: sum file...\n\n", stderr);

    fputs("Sum and count blocks in a file\n", stderr);

    exit(EINVAL);
}