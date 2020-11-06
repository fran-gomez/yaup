#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void print_help();

int main(int argc, char const *argv[])
{
    
    return EINPROGRESS;
}

void print_help() {
    fputs("usage: dd OPERAND\n\n", stderr);

    fputs("Convert and copy a file according to the operands\n", stderr);
    fputs("\tbs=BYTES read and write up to BYTES bytes at a time\n", stderr);
    fputs("\tif=FILE read from FILE instead of stdin\n", stderr);
    fputs("\tof=FILE write to FILE instead of stdout\n", stderr);
    fputs("\tcount=N copy only N input blocks\n", stderr);

    exit(EINVAL);
} 