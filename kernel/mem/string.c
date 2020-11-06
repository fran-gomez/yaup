#include "../include/string.h"

void *memset(void *dst, int c, size_t sz) {

    while (--sz >= 0)
        *((int*)dst+sz) = c;
    
    return dst;
}

int memcmp(void *s1, void *s2, size_t sz) {
    
}