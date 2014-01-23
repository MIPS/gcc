#include <sys16.h>

extern int main(int, char **);
extern void exit(int);

void _start(void)
{
	exit(main(SYS->argc, SYS->argv));
}
