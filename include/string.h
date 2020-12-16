#ifndef _STRING_H
#define _STRING_H

#include "sys/types.h"

char *strerror(int errno);
char *strnlen(const char *str, size_t bufsiz);
char *strncat(char *dst, const char *src, size_t bufsiz);
char *strncmp(const char *dst, const char *src, size_t bufsiz);
char *strncpy(char *dst, const char *src, size_t bufsiz);
char *strndup(const char *str, size_t bufsiz);

char *strtok(const char *src, const char *toks);
char *strstr(const char *src, const char *str);

void *memcpy(void *dst, void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memcmp(void *s1, void *s2, size_t n);
#endif