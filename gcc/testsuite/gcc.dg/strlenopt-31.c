/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-strlen" } */

#include "strlenopt.h"

volatile int v;

size_t
f1 (void)
{
  char a[30];
  v += 1;
  memcpy (a, "1234567", 7);
  memcpy (a + 7, "89abcdefg", 9);
  memcpy (a + 16, "h", 2);
  return strlen (a);	// This strlen should be optimized into 17.
}

size_t
f2 (char *a)
{
  v += 2;
  memcpy (a, "1234567", 7);
  memcpy (a + 7, "89abcdefg", 9);
  memcpy (a + 16, "h", 2);
  return strlen (a);	// This strlen should be optimized into 17.
}

size_t
f3 (void)
{
  char a[30];
  v += 3;
  a[0] = '1';
  memcpy (a + 1, "2345678", 8);
  return strlen (a);	// This strlen should be optimized into 8.
}

size_t
f4 (char *a)
{
  v += 4;
  a[0] = '1';
  memcpy (a + 1, "2345678", 8);
  return strlen (a);	// This strlen should be optimized into 8.
}

int
main ()
{
  char a[30];
  if (f1 () != 17 || f2 (a) != 17 || f3 () != 8 || f4 (a) != 8)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "strlen \\(" 0 "strlen" } } */
