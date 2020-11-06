#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <err.h>

#include "login.h"

#define FALSE 0
#define TRUE  1

void usage(void);
void parse_groups(char **, const char *);
int user_exists(const char *);


int main(int argc, char const *argv[]) {

    int mflag, dflag, i;
    gid_t gid;
    char *shell, *home_dir, **gps, *user_name;
    char *user_line;
    FILE *passwd;

    user_name = argv[i];
    if (user_exists(user_name)) {
        fprintf(stderr, "useradd: user %s: already exists.\n");
        exit(EINVAL);
    }

    i = 1;
    while (i++ < argc-1)
        if (argv[i][0] == '-')
            switch (argv[i][1]) {
                case 'm':
                    mflag = TRUE;
                    break;
                case 'D':
                    dflag = TRUE;
                    break;
                case 'G':
                    gid = find_gid(argv[++i]);
                    break;
                case 'g':
                    gps = (char**) malloc(sizeof(char*) * 32);
                    if (!gps)
                        exit(EACCES);
                    parse_groups(gps, argv[++i]);
                    break;
                case 'h':
                    home_dir = argv[++i];
                    break;
                case 's':
                    shell = argv[++i];
                    break;
                default:
                    fprintf(stderr, "useradd: %s: option not recognized\n", argv[i]);
                    usage();
            }

    user_line = (char*) malloc(sizeof(char) * MAX_USER_LINE);
    if (!user_line) {
        fprintf(stderr, "useradd: malloc(): %s\n", strerror(errno));
        exit(errno);
    }

    // user:pass_type:uid:gid:UNUSSED:home:shell
    sprintf(user_line, "%s%s:%d:%d::%s:%s", user_name, dflag? ":":":x", get_uid(), gid, home_dir, shell);

    passwd = fopen(PASSWD_FILE, "a+");
    if (!passwd) {
        fprintf(stderr, "useradd: /etc/passwd: %s\n", strerror(errno));
        exit(errno);
    }

    fputs(user_line, passwd);
    fclose(passwd);

    return 0;
}

void usage(void) {

    fputs("usage: useradd [OPTIONS] username...\n\n", stderr);

    fputs("Create a new user account. Options are:\n", stderr);
    fputs("\t-G group\n", stderr);
    fputs("\t-s shell\n", stderr);
    fputs("\t-h home dir\n", stderr);
    fputs("\t-g groups\n", stderr);
    fputs("\t-m", stderr);
    fputs("\t-D", stderr);
}