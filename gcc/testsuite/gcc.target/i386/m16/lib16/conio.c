#include <string.h>
#include <sys16.h>

int write(int fd, const void *buf, unsigned int count)
{
	const char *p = buf;
	unsigned int c = count;

	while (c--)
		asm volatile("outb %0,$0xe9" : : "a" (*p++));

	return count;
}

void puts(const char *s)
{
	/* XXX: should loop over this */
	write(1, s, strlen(s));
}
