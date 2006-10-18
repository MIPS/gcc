/* { dg-do compile } */
/* { dg-options "-O2" } */

/* This test is based on a bug that was seen during development.  */

struct S
{
  int c: 23;
  __attribute__ ((packed)) _Decimal128 d;
};

extern struct S s;
extern struct S a[5];
extern int fails;

void
check (void)
{
  if (a[2].d != (__typeof__ (s.d)) - 28415.890625DL)
    ++fails;
}
