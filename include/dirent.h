#ifndef _DIRENT_H
#define _DIRENT_H

#include "sys/types.h"

#define BUFSIZ 512  /* Internal buffer size */
#define _NDIRS 20   /* Max opened directories */
#define NAME_MAX 65 /* Max name length + null termination */

#ifndef DIR
struct _iodir {

} _dirbuff[_NDIRS];
#endif

#define DT_DIR 0
#define DT_REG 1

struct dirent {
    ino_t d_ino; /* Inode number */
    off_t d_off; /* Not an offset */
    unsigned short d_reclen; /* Length of this record */
    unsigned char d_type; /* Type of file */
    char d_name[64]; /* Null temrinated filename */
};

#define DIR struct _iodir

// Function headers
int alphasort(struct dirent **dst, const struct dirent **src);
DIR *opendir(const char *path);
int closedir(DIR *dp);
struct dirent *readdir(DIR *dp);
void rewinddir(DIR *dp);
void seekdir(DIR *dp, long offset);
long telldir(DIR *dp);

#endif