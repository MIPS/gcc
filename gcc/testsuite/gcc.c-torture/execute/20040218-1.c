#include <stdarg.h>
extern void abort (void);
extern void exit (int);

void
test1 (int i, ...)
{
}

void
test2 (int i, int j, ...)
{
}

void
test3 (int i, int j, int k, ...)
{
}

void
test4 (int i, ...)
{
  va_list v;
  va_start (v, i);
  if (va_arg (v, double) != 1.2)
    abort ();
  va_end (v);
}

void
test5 (int i, int j, ...)
{
  va_list v;
  va_start (v, j);
  if (va_arg (v, double) != 1.2)
    abort ();
  va_end (v);
}

void
test6 (int i, int j, int k, ...)
{
  va_list v;
  va_start (v, k);
  if (va_arg (v, double) != 1.2)
    abort ();
  va_end (v);
}


int main (void)
{
  test1 (1, 1.2);
  test2 (1, 2, 1.2);
  test3 (1, 2, 3, 1.2);
  test4 (1, 1.2);
  test5 (1, 2, 1.2);
  test6 (1, 2, 3, 1.2);
  exit (0);
}
