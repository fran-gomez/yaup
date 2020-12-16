#include <stdio.h>

void usage(void);

int main(int argc, char const *argv[]) {
    
    int sflag = 0, i;
    char *name, *basename, *suffix;

    if (argc < 2)
        usage();
    
    if (argv[1][0] == '-') {
        if (argv[1][1] == 's') {
            sflag++;
            name = argv[2];
            suffix = argv[3];
        } else
            usage();
    } else
        name = argv[1];
    
    basename = name;
    while(*(++name))
        if (*name == '/')
            basename = name;
    basename++;

    // TODO: Crop the suffix from basename
    if (sflag) {
        name = basename;

        while (*basename != *suffix && *basename)
            basename++;
    }

    return 0;
}
