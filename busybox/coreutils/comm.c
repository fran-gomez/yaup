#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

int main(int argc, char const *argv[]) {
    

    return 35;
}

void print_help() {

    fputs("usage: comm file1 file2\n\n", stderr);

    fputs("Compare two sorted files line by line\n", stderr);

    exit(EINVAL);
}