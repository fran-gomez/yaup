#include <stdio.h>
#include <string.h>

#include <errno.h>

#include <dirent.h>

#include "../../include/shell.h"
#include "sh.h"

char const *builtins[] = {
    "cd",
    "clear",
    "echo",
    "env",
    "exit",
    "pwd",
    "sleep",
    "tee",
    "time",
    "who",
    0
};

int (*functions[])(char **) = {
    &yash_cd,
    &yash_clear,
    &yash_echo,
    &yash_env,
    &yash_exit,
    &yash_pwd,
    &yash_sleep,
    &yash_tee,
    &yash_time,
    &yash_who,
    0 /* You get this entry only when cmd doesnt belong to builtins */
};

char **envp_value;
char **envp_name;
int envp_size;

char *find_cmd(char *cmd);
int init_env(char **name, char **value, int envc, char const *envp[]);

int main(int argc, char const *argv[], char const *envp[]) {

    unsigned int status, i;
    char ret[12];
    char *line;
    char **cmd;

    // Initialize shell environment
    envp_name = (char**) malloc(sizeof(char*) * MAX_ENV);
    envp_value = (char**) malloc(sizeof(char*) * MAX_ENV);
    envp_size = MAX_ENV;
    init_env(envp_name, envp_value ,envp_size, envp);
    
    chdir(envp_value[HOME]);
    
    setuid(atoi(envp_value[UID]));
    setgid(atoi(envp_value[GID]));

    // Main shell loop
    while (1) {
        fprintf(stdout, "[%s]: ", envp_value[WHO]);

        line = read_line();
        if (!line)
            continue;

        cmd = split_line(line);
        if (!cmd) {
            free(line);
            continue;
        }

        i = select_cmd(builtins, *cmd);
        if (i == -1) {
            *cmd = find_cmd(*cmd);
            status = exec_cmd(cmd, envp_value);
        } else if (i >= 0) {
            status = (*functions[i])(cmd);
        } else { // i < -1
            free(line);
            free(cmd);
            continue;
        }

        sprintf(ret, "%d", status);
        strncpy(envp_value[RET], ret, 12);
        free(line);
        free(cmd);
    }

    return 0;
}

/*
  Check in each path directory for the executable
  file referenced by cmd
*/
char *find_cmd(char *cmd) {

    DIR *dp;
    struct dirent *d;
    char *token, *path, *exec;

    path = (char*) malloc(512);
    strcpy(path, envp_value[PATH]);

    token = strtok(path, ":");
    while (token) {
        if (!(dp=opendir(token))) {
            fprintf(stderr, "yash: %s: %s\n", token, strerror(errno));
            return NULL;
        }

        for (d=readdir(dp); d != NULL; d=readdir(dp)) {
            if (strcmp(d->d_name, "..") == 0 || strcmp(d->d_name, ".") == 0)
                continue;

            if (strcmp(d->d_name, cmd) == 0) {
                exec = (char*) malloc(512);
                strncpy(exec, token, 512);
                strcat(exec, "/");
                strcat(exec, cmd);
                
                free(path);
                closedir(dp);

                return exec;
            }
        }
        closedir(dp);

        token = strtok(NULL, ":");
    }
    free(path);
    fprintf(stderr, "%s: Command not found\n", cmd);

    return NULL;
}

int init_env(char **name, char **value, int envc, char const *envp[]) {

    int i = 0;

    // Initialize environment values
    while (i <= SHLL) {
        name[i] = (char*) malloc(32);
        value[i] = envp[i];
        i++;
    }

    strncpy(name[WHO], "USER", 32);
    strncpy(name[UID], "UID", 32);
    strncpy(name[GID], "GID", 32);
    strncpy(name[HOME], "HOME", 32);
    strncpy(name[PATH], "PATH", 32);
    strncpy(name[PWD], "PWD", 32);
    strncpy(name[RET], "?", 32);
    strncpy(name[SHLL], "SHELL", 32);

    // Set the rest of the table to null
    while (i < envc) {
        name[i] = NULL;
        i++;
    }

    return 0;
}