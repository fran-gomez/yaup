#include <stdio.h>
#include <string.h>

#include "sh.h"

int yash_echo(char **argv) {

    int i;

    argv++;
    // Check if we want to print an environment
    // variable, or just a string
    if (**argv == '$') {
        (*argv)++;
        
        for (i = 0; i < envp_size; i++)
            if (strcmp(envp_name[i], *argv) == 0) {
                fprintf(stdout, "%s\n", envp_value[i]);
                break;
            }
    } else {
        while (*argv) {
            while (**argv != '\0') {
                if (**argv == '"' || **argv == '\'') // Skip opening and closing chars
                    (*argv)++;
                if (**argv == '\\') { // Print special characters
                    (*argv)++;
                    fputc(**argv, stdout);
                    (*argv)++;
                    continue;
                }

                fputc(**argv, stdout);
                (*argv)++;
            }
            fputc(' ', stdout);

            argv++;
        }
        fputc('\n', stdout);
    }

    return 0;
}

int yash_clear(char **argv) {
    fprintf(stdout, "\033[H\033[J");
    return 0;
}