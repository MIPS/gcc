/* { dg-do run }  */
/* { dg-require-ifunc "" } */
/* { dg-options "-O2" } */

#include <stdio.h>

extern void abort (void);

static void *implementation (void)
{
  printf ("'ere I am JH\n");
  return 0;
}

static void *resolver (void)
{
  return (void *)implementation;
}

static int magic (void) __attribute__ ((ifunc ("resolver")));

void *
get_magic (void)
{
  return &magic;
}

int
main ()
{
  if (get_magic () != &magic)
    abort ();

  return magic () != 0;
}
