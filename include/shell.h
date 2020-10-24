#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#include <err.h>
#include <errno.h>

#define BUFF_SZ 512

char *read_line(void) {

    int c, i, bufsiz;
    char *buff;

    bufsiz = BUFF_SZ;
    buff  = (char*) malloc(sizeof(char) * bufsiz);
    if (!buff)
        return NULL;

    i = 0;
    while (1) {
        c = fgetc(stdin);

        if (c == EOF || c == '\n') {
            buff[i] = '\0';
            return buff;
        } else {
            buff[i] = c;
            i++;
        }
        
        if (i >= BUFF_SZ) {
            bufsiz += BUFF_SZ;
            buff = (char*) realloc(buff, bufsiz);
            if (!buff)
                return NULL;
        }
    }

    return NULL;
}

char **split_line(char *line) {

    int buffsize = 64, i = 0;
    char **tokens, *token;

    tokens = (char**) malloc(buffsize * sizeof(char*));
    if (!tokens)
        return NULL;
    
    token = strtok(line, " \t\n\r\a");
    while (token) {
        tokens[i] = token;
        i++;

        if (i >= buffsize) {
            buffsize += 64;
            tokens = (char**) realloc(tokens, buffsize);
            if (!tokens)
                return NULL;
        }

        token = strtok(NULL, " \t\n\r\a");
    }
    tokens[i] = NULL;

    return tokens;
}

int select_cmd(char const *builtins[], const char *cmd) {

    int i = 0;

    if (cmd == NULL)
        return -2;

    while (*builtins) {
        if (strcmp(*builtins, cmd) == 0)
            return i;
        
        i++;
        builtins++;
    }

    return -1;
}

int exec_cmd(char *argv[], char *envp[]) {

    int status;
    pid_t pid;

    pid = fork();
    if (pid == 0) { // I'm the child
        if (envp == NULL) {
            if (execv(*argv, argv) < 0)
                warn("%s", *argv);
        } else {
            if (execvpe(*argv, argv, envp) < 0)
                warn("%s", *argv);
        }
    } else if (pid > 0) // I'm the parent
        waitpid(pid, &status, 0);
    else { // Damn it, I failed
        warn("Error. Cannot create child process\n");
        return EINVAL;
    }

    return status;
}

#endif