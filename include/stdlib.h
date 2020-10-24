#ifndef _STDLIB_H
#define _STDLIB_H

#include "sys/types.h"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 32767

// Function headers
void abort();
int exit(int errno);
int system(const char *cmd);

double atof(const char *ascii);
int atoi(const char *ascii);
long atol(const char *ascii);
long long atoll(const char *ascii);

char *getenv(const char *env);

void *malloc(size_t size);
void *realloc(void *ptr, size_t nsize);
void free(void *ptr);

int rand();
int rand_r(unsigned int *seed);
void srand(unsigned int seed);

#endif