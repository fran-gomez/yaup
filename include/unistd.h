#ifndef _UNISTD_H
#define _UNISTD_H

#include "sys/types.h"

// Access constants
#define F_OK 010 /* Test for existence of file */
#define R_OK 004 /* Test for read permission */
#define W_OK 002 /* Test for write permission */
#define X_OK 001 /* Test for exec permission */

// Function headers
int access(const char *path, int permission);
int chown(const char *path, uid_t uid, gid_t gid);

int chdir(const char *path);
char *getcwd(char *buff, size_t bufsiz);

pid_t fork();
int exec(const char *prog, char const *argv[], char const *envp[]);
int nice(int);
int sleep(uint32_t);
int exit(int);

int pipe(int src, int dst);

uid_t getuid();
int setuid(uid_t uid);

gid_t getgid();
int setgid(gid_t gid);

int open(const char *path, mode_t mode);
int close(int fd);
int read(int fd, char *buff, size_t bufsiz);
int write(int fd, char *buff, size_t bufsiz);
int seek(int fd);

int mount(const char *name, const char *special, int rwflag);
int umount(const char *special);

#endif