/* { dg-do compile } */ 
/* { dg-options "-O2 -ftree-elim-checks -fdump-tree-elimchk-all" } */

void bar (void);

void foo ()
{
  int i;
  for (i = 0; i < 10; i++)
    {
      bar ();

      /* The following call to "bar ()" should be removed.  */
      if (i == -5) bar ();
    }
}

/* { dg-final { scan-tree-dump-times "Replacing one of the conditions" 1 "elimchk"} } */ 
