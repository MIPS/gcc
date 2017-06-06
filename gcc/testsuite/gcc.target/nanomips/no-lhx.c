/* Test nanoMIPS LHX/S instruction */
/* { dg-do compile } */
/* { dg-options "-mgp32" } */

/* { dg-final { scan-assembler-not "\tlhx\t" } } */
/* { dg-final { scan-assembler-not "\tlhxs\t" } } */

unsigned short test (unsigned short *a, int index)
{
  return a[index];
}
