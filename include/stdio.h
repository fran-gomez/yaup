#ifndef _STDIO_H
#define _STDIO_H

#include "sys/types.h"

#define BUFSIZ 512 /* Internal buffer size */
#define _NFILE 20  /* Max opened files */

#ifndef FILE
struct _iobuf {
    char *_ptr;  /* File buffer */
    off_t _cnt;  /* Offser from file start */
    char *_base;
    u8_t _flag; /* File flags */
    char _file; /* File descriptor */
} _iob[_NFILE]; // stdio internal buffer
#endif

// File flags
#define _IOREAD  01 /* Read mode */
#define _IOWRT   02 /* Write mode */
#define _IONBF   04 /* Non buffered IO */
#define _IOMYBUF 010 /* User provided buffer */
#define _IOEOF   020 /* End of file reached */
#define _IOERR   040 /* Input/Output error */
#define _IOSTRG  0100 /* ?? */
#define _IORW    0200 /* Read/Write IO */

// Common values
#define NULL 0
#define FILE struct _iobuf
#define EOF (-1)

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

// Seek values
#define SEEK_SET 0 /* Seek relative to start position */
#define SEEK_CUR 1 /* Seek relative to current position */
#define SEEK_END 2 /* Seek relative to end of file */

// Macros
#define printf(fmt, ...) fprintf(stdout, fmt, ...)
#define scanf(fmt, ...)  fscanf(stdin, fmt, ...)

#define getc()  fgetc(stdin)
#define putc(c) fputc(c, stdin)
#define gets(b, n) fgets(b, n, stdin)
#define puts(s) fputs(s, stdin)

// Function headers
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
int feof(FILE *fp);
int ferror(FILE *fp);
int fflush(FILE *fp);
int rewind(FILE *fp);

int fgetc(FILE *fp);
int fgets(char *buff, size_t bufsiz, FILE *fp);
int fputc(int c, FILE *fp);
int fputs(char *buff, FILE *fp);

int fprintf(FILE *fp, const char *fmt, ...);
int fscanf(FILE *fp, const char *fmt, ...);

int setbuff(FILE *fp, char *buff);

int getdelim(char *buff, size_t bufsiz, int delim, FILE *fp);
int getline(char *buff, size_t bufsiz, FILE *fp);

int rename(const char *oldpath, const char *newpath);
int remove(const char *path);

#endif