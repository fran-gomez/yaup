#ifndef _STAT_H
#define _STAT_H

#include "types.h"

struct stat {

    dev_t  st_dev; /* Device number */
    ino_t  st_ino; /* Inode number */
    mode_t st_mode; /* Mode RWX for user, group and others */
    short  st_nlink; /* # of links */

    uid_t st_uid; /* User ID */
    gid_t st_gid; /* Group ID */
    dev_t st_rdev; /* Device id if char or block special */
    off_t st_size; /* Size in bytes */

    time_t st_atime; /* Last access time */
    time_t st_ctime; /* Creation time */
    time_t st_mtime; /* Modification time */
};

// Usefull constants
#define S_IFMT 0170000      /* File type format */
#define     S_IFDIR 0040000 /* Directory bit */
#define     S_IFCHR 0020000 /* Char device bit */
#define     S_IFBLK 0060000 /* Block device bit */
#define     S_IFREG 0100000 /* Regular file bit */

// Permission constants
#define S_ISUID 0004000 /* Set user id */
#define S_ISGID 0002000 /* Set group id */
#define S_ISVTX 0001000 /* ?? */

#define S_IRUSR 0000400 /* Read (user) */
#define S_IWUSR 0000200 /* Write (user) */
#define S_IEUSR 0000100 /* Exec (user) */

#define S_IRGRP 0000040 /* Read (group) */
#define S_IWGRP 0000020 /* Write (group) */
#define S_IEGRP 0000010 /* Exec (group) */

#define S_IROTH 0000004 /* Read (others) */
#define S_IWOTH 0000002 /* Write (others) */
#define S_IEOTH 0000001 /* Exec (others) */

// Test macros
#define S_ISDIR(m) ((m & S_IFMT) == S_IFDIR) /* Check if dir */
#define S_ISCHR(m) ((m & S_IFMT) == S_IFCHR) /* Check if char device */
#define S_ISBLK(m) ((m & S_IFMT) == S_IFBLK) /* Cehck if block device */
#define S_ISREG(m) ((m & S_IFMT) == S_IFREG) /* Check if regular file */

// Function headers
int chmod(const char *path, mode_t mode);
int mkdir(const char *path, mode_t mode);
int stat(const char *path, struct stat *st);
int umask(mode_t mode);

#endif