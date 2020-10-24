#include <stdio.h>
#include <string.h>

#include <errno.h>

#include "../../include/shell.h"
#include "env.h"

#define LOGIN_FILE "/etc/passwd"
#define SHELL_CMD  "../bin/sh"
#define SHELL_ARGS 0

char *find_user(char *username, FILE *login_file);
int match(char *user_line, char *passwd);
void set_envp(char *user_line, char **envp);

int main(void) {
    
    FILE *fp;
    char *username, *passwd;
    char *user_line;
    char *login_cmd[] = {SHELL_CMD, SHELL_ARGS};
    char **envp;
    
    fp = fopen(LOGIN_FILE, "r");
    if (!fp)
        fprintf(stderr, "login: /etc/passwd: %s\n", strerror(errno));
    
    while (1) {
        rewind(fp);
        fprintf(stdout, "(username): ");
        username = read_line();

        if ((user_line=find_user(username, fp)) != NULL) {
            fprintf(stdout, "(password): ");
            passwd = read_line();

            if (match(user_line, passwd)) {
                envp = (char**) malloc(sizeof(char*) * MAX_ENV);
                set_envp(user_line, envp);
                exec_cmd(login_cmd, envp);
            }

            free(passwd);
            free(user_line);
        }
        
        free(username);
    }

    return 0;
}

char *find_user(char *username, FILE *login_file) {

    size_t buff_sz = 512;
    char *user_line, *token;
    char **line;

    user_line = (char*) malloc(buff_sz);
    if (!user_line) {
        fprintf(stderr, "login: %s\n", strerror(errno));
        return NULL;
    }

    line = (char**) malloc(sizeof(char*));
    if (!line) {
        fprintf(stderr, "login: %s\n", strerror(errno));
        return NULL;
    }
    // user:password:uid:gid:UNUSSED:home:shell
    while (!feof(login_file)) {
        getline(line, &buff_sz, login_file);
        strncpy(user_line, *line, 512);
        
        token = strtok(*line,  ":");
        if (strcmp(token, username) == 0) {
            return user_line;
        }
    }

    fprintf(stderr, "login: %s: User not found\n", username);
    return NULL;
}

// Set the environment values for the shell execution
void set_envp(char *user_line, char **envp) {

    int i;

    for (i = 0; i <= SHLL; i++) {
        envp[i] = (char*) malloc(256);
    }

    strncpy(envp[WHO], strtok(user_line, ":"), 256);
    strtok(NULL, ":"); // Skip password field
    strncpy(envp[UID], strtok(NULL, ":"), 256);
    strncpy(envp[GID], strtok(NULL, ":"), 256);
    strncpy(envp[HOME], strtok(NULL, ":"), 256);
    strncpy(envp[PATH], "/bin:/usr/bin:/sbin:/usr/sbin:/srv/bin:/srv/sbin", 256);
    strncpy(envp[PWD], envp[HOME], 256);
    strncpy(envp[RET], "0", 256);
    strncpy(envp[SHLL], strtok(NULL, ":"), 256);

    for (i = SHLL+1; i < MAX_ENV; i++)
        envp[i] = NULL;
}

/*
  Matches the input password with the real passwrod
  Before: userline=passwd:uid:gid:UNUSSED:home:shell
  After: userline=uid:gid:UNUSSED:home:shell
*/
int match(char *user_line, char *passwd) {

    char *user_passwd;

    user_passwd = (char*) malloc(512);
    if (!user_passwd) {
        fprintf(stderr, "login: %s\n", strerror(errno));
        return 0;
    }

    user_passwd = strtok(NULL, ":");
    if (strcmp(user_passwd, passwd) == 0) {
        //free(user_passwd);
        return 1;
    } else {
        //free(user_passwd);
        fprintf(stderr, "login: Password dont match\n");
        return 0;
    }
}