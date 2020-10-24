#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>

int count(char **files) {
    int i = 0;

    while (*files++)
        i++;
    
    return i;
}

int yash_tee(char **argv) {

    int i, c, nfiles;
    FILE **outs;

    nfiles = count(++argv);
    outs = (FILE**) malloc(sizeof(FILE) * nfiles);

    // Open all output files
    for (i = 0; i < nfiles; i++) {
        outs[i] = fopen(argv[i], "w");
        if (!outs[i]) {
            fprintf(stderr, "tee: %s: %s\n", argv[i], strerror(errno));

            while (i >= 0)
                fclose(outs[i--]);
                
            free(outs);
            return errno;
        }
    }

    // Print the input in stdout and files
    c = fgetc(stdin);
    while (c != EOF) {
        fputc(c, stdout);

        for (i = 0; i < nfiles; i++)
            fputc(c, outs[i]);

        c = fgetc(stdin);
    }

    for (i = 0; i < nfiles; i++)
        fclose(outs[i]);
    free(outs);

    return 0;
}