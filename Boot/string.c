#include "string.h"

int32_t strcmp(const char *cs, const char *ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}
	return __res;
}

void * memset(void *s, int c, uint32_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;
	return s;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0');

	return tmp;
}

char * strncpy(char * dest, const char *src, uint32_t count)
{
	char *tmp = dest;
	while (count-- && (*dest++ = *src++) != '\0');
	return tmp;
}

uint32_t strlen(const char * s)
{
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc);
	return sc - s;
}

char *strchr(const char *s, int32_t c)
{
	for( ; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;

	return (char *) s;
}