#ifndef _STRING_H
#define _STRING_H

#include "types.h"

void *memset(void *dst, int c, size_t sz);
void *memcmp(const void *s1, const void *s2, size_t sz);
void *memcpy(void *dst, const void *src, size_t sz);

int strncmp(const char *s1, const char *s2, size_t sz);
int strlen(const char *s);

void panic(const char *msj);

#endif