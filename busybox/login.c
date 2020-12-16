#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include "login.h"

#define FALSE 0
#define TRUE  1

int get_user_line(char **, const char *);
int has_password(const char *);
int match(const char *, const char *);
int set_environment(const char *, char **);

int main(void) {
    
    char *user_line, *username;
    char *passwd;
    char **envp;
    
    username = (char*) malloc(sizeof(char) * MAX_USER_NAME);
    if (!username)
        err(errno, "");
    
    envp = (char**) malloc(sizeof(char*) * MAX_ENV);
    if (!envp)
        err(errno, "");

    while (1) {
        fprintf(stdout, "(username): ");
        fscanf(stdin, "%s", username);

        if (!get_user_line(&user_line, username)) {
            fprintf(stderr, "login: user %s not found.\n", username);
            continue;
        }
        
        if (has_password(user_line)) {
            passwd = (char*) malloc(sizeof(char) * MAX_PASSWD);
            if (!passwd)
                err(errno, "");

            fprintf(stdout, "(password): ");
            fscanf(stdin, "%s", passwd);
            if (!match(username, passwd)) {
                fprintf(stderr, "login: incorrect password for user %s\n", username);
                continue;
            }
        }

        set_environment(user_line, envp);
        execve(envp[SHELL], NULL, envp);
    }

    free(username);
    free(user_line);

    return 0;
}

int get_user_line(char **user_line, const char *username) {

    int i, found = FALSE;
    FILE *passwd;
    char *user, *line;

    passwd = fopen(PASSWD_FILE, "r");
    if (!passwd)
        err(errno, "/etc/passwd");

    line = (char*) malloc(sizeof(char) * MAX_USER_LINE);
    if (!line)
        err(errno, "");

    user = (char*) malloc(sizeof(char) * MAX_USER_NAME);
    if (!user)
        err(errno, "");

    // user:pass_type:uid:gid:UNUSSED:home:shell
    while (!feof(passwd) && !found) {
        i = 0;
        fgets(line, MAX_USER_LINE, passwd);

        while(line[i] != ':') {
            user[i] = line[i];
            i++;
        }
        user[i] = '\0';

        if (strcmp(username, user) == 0)
            found = TRUE;
    }

    if (!found)
        free(line);
    else
        *user_line = line;

    // TODO: Actualizar logs

    free(user);
    fclose(passwd);

    return found;
}

int has_password(const char *user_line) {

    int i = 0;

    // Skip user name
    while (user_line[i] != ':')
        i++;

    return user_line[i+1] == 'x';
}

int match(const char *username, const char *password) {

    int i, found = FALSE;
    FILE *shadow;
    char *line, *buff;

    shadow = fopen(SHADOW_FILE, "r");
    if (!shadow)
        err(errno, "/etc/shadow");
    
    line = (char*) malloc(sizeof(char) * MAX_SHDW_LINE);
    if (!line)
        err(errno, "malloc");
    
    buff = (char*) malloc(sizeof(char) * MAX_SHDW_LINE);
    if (!buff)
        err(errno, "malloc");

    while (!feof(shadow) && !found) {
        i = 0;
        fgets(line, MAX_SHDW_LINE, shadow);

        while (line[i] != ':' && i < MAX_USER_NAME) {
            buff[i] = line[i];
            i++;
        }
        buff[i] = '\0';

        if (strcmp(buff, username) == 0)
            found = TRUE;
    }

    if (found) {
        i = 0;
        while (line[i] != ':') {
            buff[i] = line[i];
            i++;
        }

        return strcmp(buff, password) == 0;
    } else
        return 1;
}

int set_environment(const char *user_line, char **envp) {

    envp[WHO] = (char*) malloc(sizeof(char) * MAX_USER_NAME);
    envp[UID] = (char*) malloc(sizeof(char) * 5);
    envp[GID] = (char*) malloc(sizeof(char) * 6);
    envp[HOME] = (char*) malloc(sizeof(char) * 64);
    envp[SHELL] = (char*) malloc(sizeof(char) * 64);
    envp[PWD] = (char*) malloc(sizeof(char) * 256);
    envp[PATH] = (char*) malloc(sizeof(char) * 256);
    
    
    strcpy(envp[WHO], "user");
    strcpy(envp[UID], "1000");
    strcpy(envp[GID], "100");
    strcpy(envp[HOME], "/home");
    strcpy(envp[SHELL], "/bin/sh");
    strcpy(envp[PWD], "/");
    strcpy(envp[PATH], "/bin:/sbin:/usr/bin:/usr/sbin");
    envp[7] = NULL;

    return 0;
}