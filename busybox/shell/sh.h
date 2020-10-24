#ifndef _YASH_H
#define _YASH_H

#define YASH_CONF "/etc/yash.conf"

/* Index of environment variables */
#define WHO  0
#define UID  1
#define GID  2
#define HOME 3
#define PATH 4
#define PWD  5
#define RET  6
#define SHLL 7

#define MAX_ENV  15
#define MAX_PATH 10

/* List of builtin functions */
extern int yash_eval(char **);
extern int yash_cd(char **);
extern int yash_clear(char **);
extern int yash_echo(char **);
extern int yash_env(char **);
extern int yash_exit(char **);
extern int yash_pwd(char **);
extern int yash_sleep(char **);
extern int yash_tee(char **);
extern int yash_time(char **);
extern int yash_who(char **);

/* List of shell routines */
//char **get_env(void);

/* Internal variables */
extern char const *builtins[];
extern int (*functions[])(char **);

extern char **envp_name;
extern char **envp_value;
extern int envp_size;

#endif