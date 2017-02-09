/* { dg-options "-m32 isa_rev>=6" } */

extern void abort (void);
extern void exit (int);

int *pwarn;


void bla (void)
{
  if (!*pwarn)
    abort ();
    
  exit (0);
}

/* { dg-final { scan-assembler "\tbnezc\t\\\$" } } */
