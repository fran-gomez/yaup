#ifndef _ERRNO_H
#define _ERRNO_H

// Declaration of global errno variable
extern int errno;

// Error codes
#define EPERM   1  /* Operation not permitted */
#define ENOENT  2  /* No such file or directory */
#define ESRCH   3  /* No such process */
#define EINTR   4  /* Interrupted function */
#define EIO     5  /* IO error */
#define ENXIO   6  /* No such device or address */
#define E2BIG   7  /* Argument list too long */
#define ENOEXEC 8  /* Executable file format error */
#define EBADF   9  /* Bad file descriptor */
#define ECHILD  10 /* No child processes */
#define EAGAIN  11 /* Resource unavailable, try again */
#define ENOMEM  12 /* Not enough space */
#define EACCESS 13 /* Permission denied */
#define EFAULT  14 /* Bad address */
#define ENOTBLK 15 /* Invalid block address */
#define EBUSY   16 /* Device or resource busy */
#define EEXIST  17 /* File exists */
#define EXDEV   18 /* Cross-device link */
#define ENODEV  19 /* Device not found */
#define ENOTDIR 20 /* Not a directory */
#define EISDIR  21 /* Is a directory */
#define EINVAL  22 /* Invalid argument */
#define ENFILE  23 /* Too many files open in system */
#define EMFILE  24 /* Too many open files */
#define ENOTTY  25 /* UNUSED */
#define ETXTBSY 26 /* Text file busy */
#define EFBIG   27 /* File too large */
#define ENOSPC  28 /* No space left on device */
#define ESPIPE  29 /* Invalid seek */
#define EROFS   30 /* Read-only file system */
#define EMLINK  31 /* Too many links */
#define EPIPE   32 /* Broken pipe */

#define EDOM    33 /* Math argument out of domain function */
#define ERANGE  34 /* Result too large */

#define EINPROGRESS 35 /* Operation not yet implemented */

// Error messages
const char * const sys_errlist[] = {
    "",
    "Operation not permited",
    "No such file or directory",
    "No such process",
    "Interrupted function",
    "IO error",
    "No such device or address",
    "Argument list too long",
    "Executable file format error",
    "Bad file descriptor",
    "No child processes",
    "Resource unavailable, try again",
    "Not enough space",
    "Permission denied",
    "Bad address",
    "Invalid block address",
    "Device or resource busy",
    "File exists",
    "Cross-device link",
    "Device not found",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many files open in system",
    "Too many open files",
    "UNNUSED",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Invalid seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",

    "Math argument out of domain function",
    "Result too large",
};

#endif