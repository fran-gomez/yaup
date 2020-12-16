#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include "sh.h"

int env_list(char **argv);
int env_set(char **argv);
int env_unset(char **argv);
int env_help(char **argv);

int yash_env(char **argv) {

    int state;
    char *opt;

    if (argv[1] == NULL) {
        state = env_list(NULL);
    } else {
        opt = *(++argv);

        if (*opt++ == '-')
            switch (*opt) {
                case 'l': // List environment variables
                    state = env_list(NULL);
                    break;
                case 's':
                    state = env_set(++argv);
                    break;
                case 'u':
                    state = env_unset(++argv);
                    break;
                case 'h':
                    state = env_help(NULL);
                    break;
                default:
                    state = EINVAL;
                    env_help(NULL);
            }
    }
    return state;
}

int env_help(char **argv) {

    fputs("usage: env [OPTIONS]...\n\n", stderr);

    fputs("Set, unset or list shell environment variables\n", stderr);
    fputs("Options are:\n", stderr);
    fputs("\t-h: Print this help message\n", stderr);
    fputs("\t-l: List all environmen variables\n", stderr);
    fputs("\t-s [NAME] [VAL]: Set variable NAME with value VAL\n", stderr);
    fputs("\t-u [NAME]: Unset variable NAME\n", stderr);

    return 0;
}

int env_list(char **argv) {

    int i = 0;

    while (envp_name[i]) {
        if (envp_name[i] != NULL)
            fprintf(stdout, "%s = %s\n", envp_name[i], envp_value[i]);
        
        i++;
    }

    return 0;
}

int env_set(char **argv) {

    int i=0;
    char *value, *name;

    if (argv[0] == NULL || argv[1] == NULL) {
        fprintf(stderr, "usage: env -s [NAME] [VALUE]\n");
        return EINVAL;
    }

    // Put the whole value in a string
    name = *(argv++);
    if (!(value=(char*) malloc(256)))
        fprintf(stderr, "env set: %s\n", strerror(errno));
    
    while (*argv) {
        strcat(value, *argv);
        strcat(value, " ");
        
        argv++;
    }

    // Assign the name and value to the environment table
    i = 0;
    while (i < envp_size) {
        if (envp_name[i] == NULL) {
            envp_name[i] = (char*) malloc(32);
            strncpy(envp_name[i], name, 32);
            envp_value[i] = value;

            return 0;
        } else if (strcmp(envp_name[i], name) == 0) {
            envp_value[i] = value;
            return 0;
        } else
            i++;
    }

    fprintf(stderr, "env set: Cannot set variable.\n");
    return E2BIG;
}

int env_unset(char **argv) {

    int i;

    if (*argv == NULL) {
        fprintf(stderr, "usage: env -u [NAME]\n");
        return EINVAL;
    }

    i = 0;
    while (i < envp_size) {
        if (strcmp(envp_name[i], argv[0]) == 0) {
            if (i >= 0 && i <= 5) {
                fprintf(stderr, "env: Cannot unset yash environment variables");
                return EINVAL;
            }

            envp_name[i] = NULL;
            envp_value[i] = NULL;
            return 0;
        }

        i++;
    }
    fprintf(stderr, "env unset: Variable not found\n");

    return EINVAL;
}