/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-vectorize" } */

#define SIZE 0x10020

void clear (int *);
void consume (int *);

void
f (int *__restrict input, int count)
{
  int a[SIZE];
  clear (a);
  for (int i = 0; i < count; ++i)
    a[input[i]]++;
  consume (a);
}

/* { dg-final { scan-assembler-times {\tldrsw\tx[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]} 1 } } */
/* { dg-final { scan-assembler-times {\tldr\tw[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]} 1 } } */
/* { dg-final { scan-assembler-times {\tstr\tw[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]} 1 } } */
