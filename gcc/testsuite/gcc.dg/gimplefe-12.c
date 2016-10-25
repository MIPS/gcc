/* { dg-do compile } */
/* { dg-options "-fgimple" } */

void __GIMPLE (startwith ("tree-ccp1")) foo ()
{
  int a;
  int b;
  a = b + 2;
  return;
}
