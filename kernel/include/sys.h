#include "types.h"

/* File system calls */
int open(const char *path, mode_t mode);
int creat(const char path, mode_t mode);
int close(int fd);

int read(int fd, char *buff, size_t bufsiz);
int write(int fd, const char *buff, size_t bufsiz);
int seek(int fd, off_t off, int ref);

int link(const char *src, const char *dst);
int unlink(const char *src);

int mount(dev_t dev, const char *path);
int umount(const char *path);

int rename(const char *old, const char *new);
int remove(const char *path);

int mkdir(const char *path, mode_t mode);

/* Scheduler calls */
pid_t sys_fork(void);
int sys_exec(const char *path, char const *argv[], char const *envp[]);
void sys_exit(int code);

int sys_wait(pid_t pid, int *status);
int sys_sleep(time_t nsec);

pid_t sys_getpid(void);
pid_t sys_getppid(void);

int sys_setuid(uid_t uid);
uid_t sys_getuid(void);

int sys_nice(u8_t nice);

clock_t sys_times();

/* Memory allocator calls */