#include <string.h>

int strlen(const char *c)
{
	const char *c0 = c;

	while (*c)
		c++;

	return c - c0;
}
