/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-vrp" } */

void bar (void);

void
foo (int a)
{
  switch (a)
    {
    case 4:
      if (a >= 3)
        if (a <= 5)
          bar ();
    }
}

/* { dg-final { scan-tree-dump "Folding predicate a_. > 2 to 1" "vrp" } } */
/* { dg-final { scan-tree-dump "Folding predicate a_. <= 5 to 1" "vrp" } } */
/* { dg-final { cleanup-tree-dump "vrp" } } */
