/* { dg-do compile } */

int a, c, d;
short b;

void
fn1 ()
{
  int e;
  for (; c; c++)
    {
      for (; a; a++)
        b = (short) a || e;
      e = d;
    }
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 1 "vect" { xfail { ! vect_int } } } } */
