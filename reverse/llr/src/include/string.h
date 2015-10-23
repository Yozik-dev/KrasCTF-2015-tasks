#ifndef STRING_H
#define STRING_H

#include <common.h>

#ifndef MAX_STRING_SIZE
#define MAX_STRING_SIZE 0x60
#endif

size_t strlen(const char *);
size_t __strnlen(const char *str, size_t maxlen);
int strcmp (const char *p1, const char *p2);
size_t strspn (const char *s, const char *accept);
char *strpbrk (const char *s, const char *accept);
char *strchr (const char *s, int c_in);
char *strncpy (char *s1, const char *s2, size_t n);
char *strtok (char *s, const char *delim);

void *memset (void *dstpp, int c, size_t len);
void *memcpy(void *dstpp, const void *srcpp, size_t len);

#endif
