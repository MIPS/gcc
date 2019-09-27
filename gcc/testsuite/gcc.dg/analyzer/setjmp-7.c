#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>

extern void foo (int);

static jmp_buf env;

static void inner (void)
{
  longjmp (env, 1); /* { dg-warning "leak of 'ptr'" "" { xfail *-*-* } } */
}

static void middle (void)
{
  void *ptr = malloc (1024); /* { dg-message "allocated here"  "" { xfail *-*-* } }  */
  inner ();
  free (ptr);
}

void outer (void)
{
  int i;

  foo (0);

  i = setjmp(env);

  if (i == 0)
    {
      foo (1);
      middle ();
    }

  foo (3);
}
