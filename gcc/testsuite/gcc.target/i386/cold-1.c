/* Test whether using attribute((cold)) really turns on -Os.  Do this test
   by checking whether strcpy calls the library function rather than doing
   the move inline.  */
/* { dg-do compile } */
/* { dg-options "-O3 -march=k8 -fno-optimize-sibling-calls" } */
/* { dg-final { scan-assembler "call\t(.*)strcpy" } } */

void cold (char *) __attribute__((__cold__));

void cold (char *a)
{
  __builtin_strcpy (a, "testing 1.2.3 testing 1.2.3");
}
