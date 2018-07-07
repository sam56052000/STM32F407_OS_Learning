#ifndef __STRING_H__
#define __STRING_H__

#include "peripherals.h"

int32_t strcmp(const char *cs, const char *ct);
void *memset(void *s, int32_t c, uint32_t count);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, uint32_t count);
uint32_t strlen(const char *s);
char *strchr(const char *s, int32_t c);

#endif