#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <dirent.h>

#include <err.h>
#include <errno.h>

#include <sys/types.h>

void print_help();
static int print_all(void);
static int print_user(const char *username);
static int print_current(pid_t ppid);
static int print_process(const char *pid);


int main(int argc, char const *argv[], char const *envp[]) {
    
    int status;

    if (argc != 2)
        print_help();
    
    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'a':
                status = print_all();
                break;
            case 'u':
                status = print_user(envp[1]);
                break;
            case 'c':
                status = print_current(getppid());
                break;
            default:
                print_help();
        }
    } else
        print_help();

    if (status > 0)
        err(status, "void");

    return 0;
}



static int print_process(const char *pid) {

    FILE *fp;
    char status[32];
    char cmdline[64];

    sprintf(status, "/proc/%s/status", pid);
    sprintf(cmdline, "/proc/%s/cmdline", pid);

    if (!(fp=fopen(status, "r")))
        return -1;
    



    fclose(fp);
    if (!(fp=fopen(cmdline, "r")))
        return -1;

    
    fclose(fp);
}

void print_help() {

    fputs("usage: ps -[a/u/c]...\n\n", stderr);

    fputs("Print the process status of current processes\n", stderr);
    fputs("Options are:\n", stdout);
    fputs("\t-a: Print all processes\n", stdout);
    fputs("\t-u: Print only user processes\n", stdout);
    fputs("\t-c: Print current process and its children\n", stdout);
}