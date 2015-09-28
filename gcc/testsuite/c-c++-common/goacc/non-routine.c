/* This program validates the behavior of acc loops which are
   not associated with a parallel or kernles region or routine.  */

/* { dg-do compile } */

int
main ()
{
  int i, v = 0;

#pragma acc loop gang reduction (+:v) /* { dg-error "acc loops must be associated with an acc region or routine" } */
  for (i = 0; i < 10; i++)
    v++;

  return v;
}
