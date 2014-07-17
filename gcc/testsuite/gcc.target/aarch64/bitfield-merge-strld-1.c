/* { dg-do compile } */
/* { dg-options "-O2" } */

typedef unsigned long long ull;

struct foo
{
  ull a:1;
  ull b:1;
  ull c:1;
};

void bar(struct foo *a, struct foo *b)
{
  a->a = b->a;
  a->b = b->b;
  a->c = b->c;
}

/* { dg-final { scan-assembler-times "ldrb\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 2 } }  */
/* { dg-final { scan-assembler-times "strb\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 1 } }  */

