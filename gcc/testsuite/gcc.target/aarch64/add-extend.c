/* { dg-do compile } */
/* { dg-options "-O2" } */

long long
f1 (int j, int i)
{
  /* { dg-final { scan-assembler "add\tw0, *w1, *w0, *lsl *2" } } */
 unsigned t = i + (j << 2);
 unsigned long long tt = t;
 return tt;
}
